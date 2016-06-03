#include <gtkmm.h>
#include "../header/SFMLWidget.h"
#include <iostream>
#include <SFML/Graphics.hpp>



SFMLWidget * finestra;


void disegno()
{
	finestra->invalidate();
	finestra->renderWindow.clear();
	sf::CircleShape sap(50);
	sap.setPosition(sf::Vector2f(0,0));
	sap.setFillColor(sf::Color::Green);
	finestra->renderWindow.draw(sap);
	finestra->renderWindow.display();
}

 void resize_view()
    {
        // Let the View fit the pixels of the window.
        sf::Vector2f lower_right(finestra->renderWindow.getSize().x,
                                 finestra->renderWindow.getSize().y);

        sf::View view(lower_right * 0.5f,
                      lower_right);
        finestra->renderWindow.setView(view);
    }

int main(int argc, char* argv[])
{
    Gtk::Main kit(argc, argv); //Initialize Gtk

    Gtk::Window * window; //The GTK window will be our top level Window
    Gtk::Box * box;

    //Our RenderWindow will never be below  640x480 (unless we explicitly change it)
    //but it may be more then that
    
	SFMLWidget ourRenderWindow(sf::VideoMode(300, 300));
	finestra = &ourRenderWindow;
    // Doesn't draw the renderWindow but makes it so it will draw when we add it to the window
	 ourRenderWindow.signal_draw().connect(sigc::bind_return(
                                     sigc::hide(
                                         sigc::ptr_fun(&disegno)),
                                     true));

	 ourRenderWindow.signal_size_allocate().connect(sigc::hide(
                                                    sigc::ptr_fun(&resize_view)));
    ourRenderWindow.show();

	
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui.glade");


    builder->get_widget("window1", window);
    builder->get_widget("box1", box);


    window->show_all();

//    Gtk::Button ourButton("Hello I do nothing"); //Just a clickable button, it won't be doing anything
 // ourButton.show();

    box->pack_start(ourRenderWindow); 

    //PACK_SHRINK makes the VBox only allocate enough space to show the button and nothing more
  //  box->pack_start(ourButton, Gtk::PACK_SHRINK);
   // box->show();

    Gtk::Main::run(*window); //Draw the window
    return 0;
}
