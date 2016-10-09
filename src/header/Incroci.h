#ifndef INCROCI_INCLUDE
#define INCROCI_INCLUDE

#include "Blocco.h"
#include <SFML/Graphics.hpp>
#include "Direzione.h"

using namespace sf;

/** Classe generica Incrocio. */
class Incroci: public Blocco
{
protected:
	/** Coordinate del punto in cui i veicoli possono cambiare direzione nell'incrocio. */
	Vector2i posChangeDirTopLeft,	// Posizionato nel quadrante in alto a sinistra.
		posChangeDirTopRight,		// Posizionato nel quadrante in alto a destra.
		posChangeDirBottomLeft,		// Posizionato nel quadrante in basso a sinistra.
		posChangeDirBottomRight;	// Posizionato nel quadrante in basso a destra.

	/** Collider dei semafori. */
	IntRect semaphore1 = IntRect(getPos(), Vector2i(2, 2)), //Posizionato a sinistra (in alto o  in basso).
			semaphore2 = IntRect(getPos(), Vector2i(2, 2));	//posizionato a destra (in alto o in basso).

	/** Sprite per visualizzare lo stato del semaforo. 
	*	1 e 2 sono in alto, rispettivamente sinistra e destra.
	*	3 e 4 sono in basso, rispettivamente sinistra e destra.
	*/
	CircleShape semShape1, semShape2, semShape3, semShape4;

	/** Rappresenta lo stato del semaforo. Se true blocca le strade orizzontali. */
	bool blockHoriz = false;

	/** Direzione non valida. Indica la direzione in cui un veicolo non può andare. */
	Direzione wrongDir = Direzione::ND;

	/** Imposta le coordinate in cui i veicoli possono svoltare.\nViene richiamata nel costruttore dell'incrocio. */
	void setChangeDirPos();
protected:

#ifdef DEBUG_MODE
	/** Indicatori visivi di debug delle posizioni di cambio direzione della macchina. */
	RectangleShape shapeTopLeft, shapeTopRight, shapeBottomLeft, shapeBottomRight;
#endif // DEBUG_MODE

	/** Inizializza i quattro semafori. */
	void initSemShapes();

	/** Inizializza il singolo semaforo, impostando posizione e dimensione. 
	*
	*	@param semShape Sprite del semaforo.
	*	@param pos Posizione del semaforo.
	*/
	void initSemShape(CircleShape &semShape, Vector2i pos);

	/** Imposta i colori dei semafori basandosi sullo stato attuale. */
	void setSemaphoresColor();

	/** Sposta i due collider sui semafori rossi. */
	void setSemaphorePos();

public:
	/** Passa lo stato del semaforo a quello successivo. */
	void changeSemaphoreStatus();

	/** Restituisce il collider relativo al semaforo basandosi sulla direzione passata.
	*
	*	@param Direzione.
	*	@return Collider semaforo.
	*/
	IntRect getSemaphore(Direzione dir);

	/** Restituisce la direzione che deve prendere la macchina per essere instradata correttamente.
	*
	*  	@param pos Posizione attuale della macchina.
	* 	@param dir Direzione del nodo successivo all'incrocio.
	* 	@return Direzione che la macchina deve prendere per fare correttamente l'incrocio.
	*/
	Direzione getChangeDir(Vector2f pos, Direzione dir);
};
#endif // !INCROCI_INCLUDE