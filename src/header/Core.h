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

/** Imposta il numero massimo di macchine da spawnare.
*
*	@param num Numero di macchine.
*/
void setNumCar(int num);

/** Imposta il tempo che deve trascorrere fra lo spawn di una macchina e un'altra.
*
*	@param ms Tempo in millisecondi.
*/
void setSpawnCarTime(int ms);

/** Imposta il tempo che deve trascorrere per far passare i semafori al prossimo stato.
*
*	@param s Tempo in secondi.
*/
void setSemChangeStatusTime(int s);