/*
●Una descrizione generale di cosa fa il
programma
● Una prima descrizione generale di come lo fa,
nonché la suddivisione in moduli del programma
stesso
● Un rimando alla funzione (probabilmente la
funzione main) la cui documentazione
continuerà a guidare il lettore
nell'approfondimento della conoscenza del
programma
*/

/** @mainpage Traffic-Simulator.
		Questo programma gestisce il traffico stradale di un ambiente generato casualmente, nella seguente modalità:\n
		<ul>
			<li>Posizionamento delle sorgenti</li>
			<li>Collegamento delle sorgenti fra di loro
				<ul><li>Se rimangono sorgenti scollegate, queste vengono collegate con altre sorgenti</li></ul>
			</li>
		</ul>
		Consultare ::Mappa per maggiori informazioni.\n\n
		I veicoli procederanno da un luogo di partenza verso una destinazione autonomamente, rispettando le intersezioni regolate da semafori.\n
		La logica dell'applicazione è gestita da <a href="Core_8cc.html">Core</a>.

    @author Daniele Corradini
    @author Alberto Beccari
*/

#include <gtkmm.h>
#include "../header/SFMLWidget.h"
#include "../header/Core.h"
#include "../header/Debug.h"
#include <iostream>
#include <SFML/Graphics.hpp>

#ifdef DEBUG_MODE
unsigned int MASK = 0 ;
#endif // DEBUG_MODE

/** Widget SFML che verrà applicato al box GTK. */
SFMLWidget * SFML;

/** Box che contiene i menu e widget SFML */
Gtk::Box * box = NULL;

/** Window principale. */
Gtk::Window * window = NULL;

int RESX = 1280, /** Larghezza della finestra */
	RESY = 743;/**Altezza della finestra */

/** Disegna il prossimo frame, avanza la logica del gioco e dice di ignorare l'input se
*	la finestra non è più la finestra attiva.
*/
void updateCore();

/** La vista del widget viene aggiustata quando viene ridimensionato. */
void resize_view();

/** Gestore dell'apertura/chiusura della finestra delle informazioni del programma. 
*
*	@param dialog Dialog da gestire.
*/
void aboutDialogHandler(Gtk::AboutDialog *& dialog);

/** Chiude il programma con esito 0. Usato per il pulsande del menu 'Esci'. */
void exitHandler();

/** Connette i segnali relativi al menu della finestra principale.
*
*	@param builder Builder per ottenere gli oggetti che compongono il menu.
*/
void linkMenuSignals(Glib::RefPtr<Gtk::Builder> &builder);

/** Connette i segnali relativi ai widget della finestra di configurazione. 
*
*	@param builder Builder per ottenere i widget.
*	@param confDialog Dialog di configurazione.
*/
void linkConfSignals(Glib::RefPtr<Gtk::Builder> &builder, Gtk::Dialog * confDialog);

/** Crea il widget SFML e imposta la finestra principale.  
*
*	@param builder Builder per ottenere la finestra principale.
*	@param width Larghezza finestra.
*	@param height Altezza finestra.
*/
void windowInit(Glib::RefPtr<Gtk::Builder> &builder, int width, int height);

/** Gestisce l'inserimento del testo nelle caselle in modo da accettare solo numeri.
*
*	@param textEntered Testo inserito.
*	@param pos Posizione su cui si trova l'ultimo carattere inserito.
*	@param entry Casella da gestire.
*/
void entryHandler(const Glib::ustring& textEntered, int* pos, Gtk::Entry * entry);

/** Conferma le configurazioni scelte, impostandole e facendo visualizzare la finestra di gioco.
*
*	@param builder Builder per ottenere i widget.
*	@param dialog Puntatore al dialog della configurazione.
*/
void okHandler(Glib::RefPtr<Gtk::Builder> &builder, Gtk::Dialog * & dialog);

int main(int argc, char* argv[])
{
	//Inizializzazione GTK
    Gtk::Main kit(argc, argv);

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui.glade");

	Gtk::Dialog * confDialog = NULL;
	builder->get_widget("confdialog", confDialog);
	linkConfSignals(builder, confDialog);

	builder->get_widget("window1", window);

    Gtk::Main::run(*confDialog);
    return 0;
}

void updateCore()
{
	//Diciamo a GTK di ridisegnare il widget SFML
	SFML->invalidate();

	//Pulizia della SFML window
	SFML->renderWindow.clear();

	enableInput(box->has_focus());
	update(SFML->renderWindow);

	//Si visualizza ciò che è stato disegnato
	SFML->renderWindow.display();
}

void resize_view()
{
	Vector2f size(SFML->renderWindow.getSize().x, SFML->renderWindow.getSize().y);
	Vector2f center(size.x / 2, size.y / 2);

	View view(center, size);
	SFML->renderWindow.setView(view);
}

void aboutDialogHandler(Gtk::AboutDialog *& dialog)
{
	int response = dialog->run();

	if (response == Gtk::RESPONSE_DELETE_EVENT || response == Gtk::RESPONSE_CANCEL)
		dialog->hide();
}

void exitHandler()
{
	exit(0);
}

void linkMenuSignals(Glib::RefPtr<Gtk::Builder> &builder)
{
	Gtk::ImageMenuItem * aboutButton = NULL;
	builder->get_widget("aboutbutton", aboutButton);

	Gtk::AboutDialog * aboutDialog = NULL;
	builder->get_widget("infodialog", aboutDialog);
	aboutButton->signal_activate().connect(sigc::bind<Gtk::AboutDialog *>(&aboutDialogHandler, aboutDialog));

	Gtk::ImageMenuItem * exitButton = NULL;
	builder->get_widget("exitbutton", exitButton);
	exitButton->signal_activate().connect(sigc::ptr_fun(&exitHandler));

	Gtk::ImageMenuItem * refreshButton = NULL;
	builder->get_widget("refreshbutton", refreshButton);
	refreshButton->signal_activate().connect(sigc::ptr_fun(&refreshMap));
}

void linkConfSignals(Glib::RefPtr<Gtk::Builder> &builder, Gtk::Dialog * confDialog)
{
	Gtk::Button * button = NULL;
	builder->get_widget("annullabutton", button);
	button->signal_clicked().connect(sigc::ptr_fun(&exitHandler));

	builder->get_widget("okbutton", button);
	button->signal_clicked().connect(sigc::bind<Glib::RefPtr<Gtk::Builder>, Gtk::Dialog *>(&okHandler, builder, confDialog));

	Gtk::Entry * entry = NULL;
	builder->get_widget("entrycars", entry);
	entry->signal_insert_text().connect(sigc::bind(sigc::ptr_fun(&entryHandler), entry));

	builder->get_widget("entryspawn", entry);
	entry->signal_insert_text().connect(sigc::bind(sigc::ptr_fun(&entryHandler), entry));

	builder->get_widget("entrysem", entry);
	entry->signal_insert_text().connect(sigc::bind(sigc::ptr_fun(&entryHandler), entry));
}

void windowInit(Glib::RefPtr<Gtk::Builder> &builder, int width, int height)
{
	//Creazione della zona di disegno mediante SFMLWidget
	//IMPORTANTE: vengono applicate solo le modifiche al widget
	// fatte dopo il window show all
	SFML = new SFMLWidget(VideoMode(RESX, RESY));
	CoreInit();

	//Link del segnale draw del widget
	SFML->signal_draw().connect(sigc::bind_return(sigc::hide(
		sigc::ptr_fun(&updateCore)),
		true
	));
	SFML->signal_size_allocate().connect(sigc::hide(sigc::ptr_fun(&resize_view)));
	SFML->show();

	builder->get_widget("box1", box);
	box->pack_start(*SFML);

	linkMenuSignals(builder);

	window->set_size_request(width, height);
	//Si mostrano tutti i widget contenuti nella SFML principale
	window->show_all();

	SFML->renderWindow.setFramerateLimit(60);
}

void entryHandler(const Glib::ustring& textEntered, int* pos, Gtk::Entry * entry)
{
	int inputLength = textEntered.length();
	for (int i = 0; i < inputLength; i++)
	{
		if (inputLength > 1 && (textEntered[i] < '0' || textEntered[i] > '9'))
		{
			entry->set_text("");
			break;
		}

		if (textEntered[i] < '0' || textEntered[i] > '9')
			entry->set_text(entry->get_text().substr(0, entry->get_text().length() - 1));
	}
}

void okHandler(Glib::RefPtr<Gtk::Builder> &builder, Gtk::Dialog * & dialog)
{

	Gtk::Entry * entry = NULL;

	builder->get_widget("entrycars", entry);
	setNumCar(atoi(entry->get_text().c_str()));
	D1(PRINT("Numero macchine scelto: " << entry->get_text()));

	builder->get_widget("entryspawn", entry);
	setSpawnCarTime(atoi(entry->get_text().c_str()));
	D1(PRINT("Spawn time macchine scelto: " << entry->get_text()));

	builder->get_widget("entrysem", entry);
	setSemChangeStatusTime(atoi(entry->get_text().c_str()));
	D1(PRINT("Tempo scelto per il cambio di stao dei semafori: " << entry->get_text()));

	Gtk::ComboBox * combo = NULL;
	builder->get_widget("combobox1", combo);

	int separator = combo->get_active_id().find("x", 1),
		width = atoi(combo->get_active_id().substr(0, separator).c_str()),
		height = atoi(combo->get_active_id().substr(separator + 1, combo->get_active_id().length()).c_str());

	RESX = width;
	RESY = height - 25;

	dialog->hide();
	
	windowInit(builder, width, height);
	Gtk::Main::run(*window);
}