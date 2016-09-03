#include <SFML/Graphics.hpp>
#include "Debug.h"
#include "Mappa.h"
#include "Macchina.h"
#include "Graph.h"
#include "Macchina_List.h"
#include "MacchinaPtr_List.h"

using namespace sf;

void CoreInit();
void update(sf::RenderWindow &widget);

/**Aggiorna la posizione della macchina\n
*  Se la macchina si trova su una curva o su un incrocio, cambia la direzione della macchina*/
void updateCar(Macchina &car);
void placeCar(Vector2i &source, Direzione &d);
void replaceCar(Macchina &car);
void refreshMap();
void createCar();
void updateSemaphores();
void inputHandling(RenderWindow &widget);