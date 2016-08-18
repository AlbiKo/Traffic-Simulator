#ifndef INCROCI_INCLUDE
#define INCROCI_INCLUDE


#include "Blocco.h"
#include <SFML/Graphics.hpp>
#include "Direzione.h"

using namespace sf;

class Incroci: public Blocco
{

protected:
	Vector2i pos1, pos2, pos3, pos4;
public:
	//Incroci();
	//Incroci(int rowIndex, int columnIndex);

	/**Restituisce la direzione che deve prendere la macchina per essere instradata correttamente.
	*  @param pos Posizione attuale della macchina
	*  @param dir Direzione del nodo successivo all'incrocio
	*/
	Direzione getChangeDir(Vector2f pos, Direzione dir);

	void setPos(int rowIndex, int columnIndex);
};
#endif // !INCROCI_INCLUDE