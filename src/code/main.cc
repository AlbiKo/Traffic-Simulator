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
		Questo programma gestisce il traffico stradale di un ambiente generato casualmente.\n
		La "map" generata conterrà incroci che saranno gestiti da semafori.\n
		I veicoli procederanno da un luogo di partenza verso una destinazione autonomamente, rispettando gli incroci e le regolazioni semaforiche.\n
		\n Benvenuti in SimCity 2000!\n\n
		NOTA:
		Documentazione incompleta su ::Blocco



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
unsigned int MASK = 3 ;
#endif // DEBUG_MODE

/**Widget SFML che verrà applicato al box GTK
*/
SFMLWidget * SFML;

/** Box che contiene i menu e widget SFML */
Gtk::Box * box = NULL;

int RESX = 1280, /** Larghezza della finestra */
	RESY = 740;/**Altezza della finestra */

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

/** Connette i segnali relativi al menu.
*
*	@param builder Builder per ottenere gli oggetti che compongono il menu.
*/
void linkSignals(Glib::RefPtr<Gtk::Builder> &builder);

int main(int argc, char* argv[])
{
	//Inizializzazione GTK
    Gtk::Main kit(argc, argv);

	//Window principale
    Gtk::Window * window = NULL;
    

	//Creazione della zona di disegno mediante SFMLWidget
	//IMPORTANTE: vengono applicate solo le modifiche al widget
	// fatte dopo il window show all
	SFML = new SFMLWidget(sf::VideoMode(RESX, RESY));

    CoreInit();


	//Link del segnale draw del widget
	SFML->signal_draw().connect(sigc::bind_return(sigc::hide(
															sigc::ptr_fun(&updateCore)),
															true
												));
	SFML->signal_size_allocate().connect(sigc::hide(sigc::ptr_fun(&resize_view)));
	SFML->show();


    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui.glade");

    builder->get_widget("window1", window);
    builder->get_widget("box1", box);

	linkSignals(builder);

	box->pack_start(*SFML);
    //Si mostrano tutti i widget contenuti nella SFML principale
    window->show_all();

    SFML->renderWindow.setFramerateLimit(60);

    Gtk::Main::run(*window); //Draw the window
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
	sf::Vector2f lower_right(SFML->renderWindow.getSize().x,
		SFML->renderWindow.getSize().y);

	sf::View view(lower_right * 0.5f, lower_right);
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

void linkSignals(Glib::RefPtr<Gtk::Builder> &builder)
{
	Gtk::ImageMenuItem * aboutButton = NULL;
	builder->get_widget("aboutbutton", aboutButton);

	Gtk::AboutDialog * aboutDialog = NULL;
	builder->get_widget("aboutdialog1", aboutDialog);
	aboutButton->signal_activate().connect(sigc::bind<Gtk::AboutDialog *>(&aboutDialogHandler, aboutDialog));

	Gtk::ImageMenuItem * exitButton = NULL;
	builder->get_widget("exitbutton", exitButton);
	exitButton->signal_activate().connect(sigc::ptr_fun(&exitHandler));

	Gtk::ImageMenuItem * refreshButton = NULL;
	builder->get_widget("refreshbutton", refreshButton);
	refreshButton->signal_activate().connect(sigc::ptr_fun(&refreshMap));
}