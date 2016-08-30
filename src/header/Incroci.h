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

	IntRect semaphore1 = IntRect(getPos(), Vector2i(2,2)), 
			semaphore2 = IntRect(getPos(), Vector2i(2, 2));

	bool blockHoriz = false;

	Direzione wrongDir = Direzione::ND;

	/**Imposta le coordinate in cui i veicoli possono svoltare.\nViene richiamata nel costruttore dell'incrocio.*/
	void setChangeDirPos();
public:

	/**Restituisce la direzione che deve prendere la macchina per essere instradata correttamente.
	*  @param pos Posizione attuale della macchina
	*  @param dir Direzione del nodo successivo all'incrocio
	*  @return Direzione che la macchina deve prendere per fare correttamente l'incrocio.*/
	Direzione getChangeDir(Vector2f pos, Direzione dir);
	
	void setSemaphorePos();
	void changeSemaphoreStatus();
	IntRect getSemaphore(Direzione dir);
};
#endif // !INCROCI_INCLUDE