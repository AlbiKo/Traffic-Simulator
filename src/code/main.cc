
/** @mainpage Faccio la prova.
    Super prova che spacca. Davvero davvero. ::main \n



    @author Me
    @author Te
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

SFMLWidget * SFML;


int RESX = 1280,
	RESY = 700;

void disegno()
{


	//Diciamo a GTK di ridisegnare il widget SFML
	SFML->invalidate();

	//Pulizia della SFML window
	SFML->renderWindow.clear();

	update(SFML->renderWindow);

	//Si visualizza ciò che è stato disegnato
	SFML->renderWindow.display();
}

//Ogni volta che il widget viene ridimensionato, si aggiusta la View.
void resize_view ()
{
	sf::Vector2f lower_right(SFML->renderWindow.getSize().x,
							 SFML->renderWindow.getSize().y);

	sf::View view(lower_right * 0.5f, lower_right);

	SFML->renderWindow.setView(view);
}

int main(int argc, char* argv[])
{
	//Inizializzazione GTK
    Gtk::Main kit(argc, argv);

	//La SFML GTK è quella principale
    Gtk::Window * window = NULL;
    Gtk::Box * box;

	//Creazione della zona di disegno mediante SFMLWidget
	//IMPORTANTE: vengono applicate solo le modifiche al widget
	// fatte dopo il window show all
	SFML = new SFMLWidget(sf::VideoMode(RESX, RESY));

    CoreInit();


	//Link del segnale draw del widget
	SFML->signal_draw().connect(sigc::bind_return(sigc::hide(
															sigc::ptr_fun(&disegno)),
															true
												));
	SFML->signal_size_allocate().connect(sigc::hide(sigc::ptr_fun(&resize_view)));
	SFML->show();


    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui.glade");

    builder->get_widget("window1", window);
    builder->get_widget("box1", box);

    box->pack_start(*SFML);
    //Si mostrano tutti i widget contenuti nella SFML principale
    window->show_all();

    SFML->renderWindow.setFramerateLimit(60);

    Gtk::Main::run(*window); //Draw the window
    return 0;
}
