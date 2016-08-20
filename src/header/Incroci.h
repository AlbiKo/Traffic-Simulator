#ifndef INCROCI_INCLUDE
#define INCROCI_INCLUDE


#include "Blocco.h"
#include <SFML/Graphics.hpp>
#include "Direzione.h"

using namespace sf;

/** Classe generica Incrocio.*/
class Incroci: public Blocco
{

protected:
	/**Coordinate del punto in cui i veicoli possono cambiare direzione nell'incrocio.*/
	Vector2i pos1, pos2, pos3, pos4;


public:

	/**Restituisce la direzione che deve prendere la macchina per essere instradata correttamente.
	*  @param pos Posizione attuale della macchina
	*  @param dir Direzione del nodo successivo all'incrocio
	*  @return Direzione che la macchina deve prendere per fare correttamente l'incrocio.*/
	Direzione getChangeDir(Vector2f pos, Direzione dir);

	/**Imposta le coordinate in cui i veicoli possono svoltare.\nViene richiamata nel costruttore dell'incrocio.*/
	void setPos(int rowIndex, int columnIndex);
};
#endif // !INCROCI_INCLUDE