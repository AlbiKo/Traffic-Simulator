#include <SFML/Graphics.hpp>
#include "Debug.h"
#include "Mappa.h"
#include "Macchina.h"
#include "Graph.h"
#include "Macchina_List.h"

void CoreInit();
void update(sf::RenderWindow &widget);

/**Aggiorna la posizione della macchina\n
*  Se la macchina si trova su una curva o su un incrocio, cambia la direzione della macchina*/
void updateMacchina(Macchina &macchina);
void placeCar(Vector2i &source, Direzione &d);
void addCar(Vector2i source);
