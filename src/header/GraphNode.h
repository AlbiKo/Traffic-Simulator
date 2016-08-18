#ifndef GRAPHNODE_INCLUDE
#define GRAPHNODE_INCLUDE


#include <SFML/Graphics.hpp>
#include "Direzione.h"

using namespace sf;

/*
Mi serve un qualcosa che tiene traccia nel singolo nodo di:
- Posizione (identificatore del nodo)
- 4 vicini (puntatori a posizioni?)


Si potrebbe rendere il peso delle categorie di blocco diverso:
- Rect -> 1
- Curve -> 2
- Incrocio 3 -> 3
- Incrocio 4 -> 4
*/

/** La classe nodo rappresenta l'elemento base del grafo
*	utilizzato per calcolare il percorso delle macchine.
*
*	@author Quello bravo
*/

class GraphNode
{

private:

	//Indici dell'array per individuare la posizione delle adiacenze
	const int	UP_INDEX	= 0,
				DOWN_INDEX	= 1,
				LEFT_INDEX	= 2,
				RIGHT_INDEX = 3;

	/**	Posizione del nodo basandosi sui pixel della finestra.
	*	Per esempio (0,0) sarà l'angolo in alto a sinistra.
	*/
    Vector2i pos;

	/**	Struttura che rappresenta il collegamento fra due nodi.
	*	nodo -> puntatore al nodo a cui è collegato il nodo corrente
	*	peso -> quanto sono distanti i due nodi in pixel.
	*/
	struct Adiacenza
	{
		GraphNode * nodo;
		int peso;
	} adiacenze[4];

	/**	Imposta e memorizza un'adiacenza con un certo nodo nell'apposito array.
	*	Viene aggiornato il peso, calcolato come distanza in pixel fra i due nodi.
	*	@param i Posizione dell'array in cui verrà memorizzata l'adiacenza
	*	@param nodo Puntatore al nodo adiacente.
	*/
	bool setAdiacenza(int i, GraphNode * nodo, int peso);

	/** Restituisce, se esiste, il nodo adiacente e il peso del collegamento.
	*	@param i Posizione nell'array delle adiacenze
	*	@param peso Parametro di uscita: peso del collegamento
	*	@return Puntatore al nodo adiacente
	*/
	GraphNode * getAdiacenza(int i, int &peso);

	void initNodo();
public:
	/** Costruttore base.  */
    GraphNode();

    /** Costruttore che imposta la posizione di partenza  del nodo.
    *	Richiama il costruttore base.
    *	@param startPos Posizione iniziale
    */
    GraphNode(Vector2i pos);
	~GraphNode();

	void copy(GraphNode gn);

	/** Cambia la posizione del nodo.
    *	Richiama il costruttore base.
    *	@param newPos Nuova posizione
    */
	void setPos(Vector2i newPos);

	bool setAdiacenza(Direzione dir, GraphNode * nodo, int peso);

	/** Imposta alla prima posizione libera la nuova adiacenza e
	*	il peso del collegamento.
	*	@param nodo Puntatore al nodo adiacente
	*	@return True se ha trovato una posizione libera e inserito l'adiacenza.\n
	*			False in caso contrario.
	*/
	bool setAdiacenza(GraphNode * nodo);

	GraphNode * getAdiacenza(Direzione dir, int &peso);

	inline Vector2i getPos()
	{
		return pos;
	}

};

#endif // !GRAPHNODE_INCLUDE