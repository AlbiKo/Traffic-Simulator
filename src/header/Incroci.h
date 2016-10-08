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
	Vector2i posChangeDirTopLeft,	// Posizionato nel quadrante in alto a sinistra.
		posChangeDirTopRight,		// Posizionato nel quadrante in alto a destra.
		posChangeDirBottomLeft,		// Posizionato nel quadrante in basso a sinistra.
		posChangeDirBottomRight;		// Posizionato nel quadrante in basso a destra.

	IntRect semaphore1 = IntRect(getPos(), Vector2i(2,2)), 
			semaphore2 = IntRect(getPos(), Vector2i(2, 2));

	CircleShape semShape1, semShape2, semShape3, semShape4;

	bool blockHoriz = false;

	/** Direzione non valida. Indica la direzione in cui un veicolo non può andare */
	Direzione wrongDir = Direzione::ND;

	/**Imposta le coordinate in cui i veicoli possono svoltare.\nViene richiamata nel costruttore dell'incrocio.*/
	void setChangeDirPos();
public:

#ifdef DEBUG_MODE
	/** Indicatori visivi di debug delle posizioni di cambio direzione della macchina. */
	RectangleShape shapeTopLeft, shapeTopRight, shapeBottomLeft, shapeBottomRight;
#endif // DEBUG_MODE

	/** Restituisce la direzione che deve prendere la macchina per essere instradata correttamente.
	*  	@param pos Posizione attuale della macchina
	* 	@param dir Direzione del nodo successivo all'incrocio
	* 	@return Direzione che la macchina deve prendere per fare correttamente l'incrocio. */
	Direzione getChangeDir(Vector2f pos, Direzione dir);
	
	void initSemShapes();
	void initSemShape(CircleShape &semShape, Vector2i pos);
	void setSemaphoresColor();
	void setSemaphorePos();
	void changeSemaphoreStatus();
	IntRect getSemaphore(Direzione dir);
};
#endif // !INCROCI_INCLUDE