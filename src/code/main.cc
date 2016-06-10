
/** @mainpage Faccio la prova.
    Super prova che spacca. Davvero davvero. ::main \n

    Sopra altra robetta
    Eh
    Scrivo altra robetta
Blob
    Testina

Scrivo robetta

    @author Me
    @author Te
*/



#include <gtkmm.h>
#include "../header/SFMLWidget.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cassert>

#ifdef DEBUG_MODE
unsigned int MASK = 1 ;

#define DBG(A, B) {if ((A) & MASK) {B; } }
#else
#define DBG(A, B)
#endif


#define D1(a) DBG(1, a)
#define D2(a) DBG(2, a)
#define D3(a) DBG(4, a)

SFMLWidget * SFML;
sf::RectangleShape * rect;

void disegno()
{
	//Diciamo a GTK di ridisegnare il widget SFML
    static bool premuto = false;
	SFML->invalidate();

	//Pulizia della SFML window
	SFML->renderWindow.clear();

    if (!premuto && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        std::cout <<"premo S \n";
        rect->move(0, 1);
        premuto = true;
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        premuto = false;

	//Disegno del cerchio
	SFML->renderWindow.draw(*rect);

	//Si visualizza ciò che è stato disegnato
	SFML->renderWindow.display();
}

//Ogni volta che il widget viene ridimensionato, si aggiusta la View.
 void resize_view()
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

    //Asserzione che fallisce (quindi termina il programma)
    assert(window != NULL);

	//Creazione della zona di disegno mediante SFMLWidget
	//IMPORTANTE: vengono applicate solo le modifiche al widget
	// fatte dopo il window show all
	SFML = new SFMLWidget(sf::VideoMode(300, 300));

    rect = new sf::RectangleShape(sf::Vector2f(300, 100));
    rect->setFillColor(sf::Color::Green);

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
