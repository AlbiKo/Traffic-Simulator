#include <SFML/Graphics.hpp>
#include "Debug.h"
#include "Mappa.h"
#include "Macchina.h"
#include "Graph.h"
#include "Macchina_List.h"
#include "MacchinaPtr_List.h"

using namespace sf;

/** Crea la prima mappa e il relativo grafo. */
void CoreInit();

/** Abilita/disabilita l'input. */
void enableInput(bool enable);

/** Avanza la logica del gioco, disegnando gli oggetti, spawnando/creando le macchine,
*	controllando le collisioni fra macchine e gestendo l'input.
*
*	@param widget Finestra su cui disegnare gli oggetti.
*/
void update(sf::RenderWindow &widget);

/** Rigenera la mappa. */
void refreshMap();

void setNumCar(int num);

void setSpawnCarTime(int ms);

void setSemChangeStatusTime(int s);