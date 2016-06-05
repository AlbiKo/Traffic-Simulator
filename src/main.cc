
/** @mainpage Faccio la prova.
    Super prova che spacca. Davvero davvero. ::main \n
    rafds

    @author Me
    @author Te
*/


#include <gtkmm.h>
#include "../header/SFMLWidget.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cassert>

SFMLWidget * SFML;

void disegno()
{
	//Diciamo a GTK di ridisegnare il widget SFML
	SFML->invalidate();

	//Pulizia della SFML
	SFML->renderWindow.clear();

	//Creazione del cerchio da disegnare
	sf::CircleShape sap(50);
	sap.setPosition(sf::Vector2f(0,0));
	sap.setFillColor(sf::Color::Green);

	//Disegno del cerchio
	SFML->renderWindow.draw(sap);

	//Si visualizza ciò che è stato disegnato
	SFML->renderWindow.display();
}

 void resize_view()
    {
        // Let the View fit the pixels of the window.
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

    //Asserzione che fallisce (quindi termina il programma)
    //assert(window != NULL);

	//Creazione della zona di disegno mediante SFMLWidget
	SFML = new SFMLWidget(sf::VideoMode(300, 300));

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

	//Si mostrano tutti i widget contenuti nella SFML principale
    window->show_all();

    box->pack_start(*SFML);

    Gtk::Main::run(*window); //Draw the window
    return 0;
}
