#include <SFML/Graphics.hpp>
#include "Debug.h"
#include "Mappa.h"
#include "Macchina.h"
#include "Graph.h"
#include "Macchina_List.h"
#include "MacchinaPtr_List.h"

using namespace sf;

void CoreInit();
void enableInput(bool enable);
void update(sf::RenderWindow &widget);
void refreshMap();