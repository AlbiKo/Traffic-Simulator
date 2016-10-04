#ifndef GRAPHNODE_INCLUDE
#define GRAPHNODE_INCLUDE

#include <SFML/Graphics.hpp>
#include "Direzione.h"

using namespace sf;

/** La classe ::GraphNode rappresenta il nodo del grafo ::Graph utilizzato per calcolare il percorso delle macchine.
*	Il nodo è costituito dalla posizione, che funge da identificatore, e dalle adiacenze verso le quattro direzioni,
*	ognuna costituita dal puntatore al nodo adiacente e dal peso del collegamento.
*/

class GraphNode
{
private:

	/** Indici costanti per individuare la posizione delle adiacenze nell'array riferendosi ad una certa direzione. */
	const int	UP_INDEX	= 0,
				DOWN_INDEX	= 1,
				LEFT_INDEX	= 2,
				RIGHT_INDEX = 3;

	/**	Struttura che rappresenta il collegamento fra due nodi.
	*
	*	nodo -> Puntatore al nodo a cui è collegato il nodo corrente.
	*	peso -> Peso del collegamento.
	*/
	struct Adiacenza
	{
		GraphNode * nodo;
		int peso;
	} adiacenze[4];

	/** Inizializza la posizione e l'array di adiacenze del nodo. */
	void initNodo();

	/**	Imposta l'adiacenza con il nodo passato come parametro all'indice specificato dell'array solo se in quella posizione non esista già un'adiacenza.
	*
	*	@param i Posizione dell'array in cui verrà memorizzata l'adiacenza.
	*	@param nodo Puntatore al nodo adiacente.
	*	@param peso Peso del collegamento.
	*	@return True se ha impostato l'adiacenza. False altrimenti.
	*/
	bool setAdiacenza(int i, GraphNode * nodo, int peso);

public:
	/**	Posizione del nodo. */
	Vector2i pos;

	/** Costruttore base. */
    GraphNode();

    /** Costruttore che imposta la posizione del nodo.
    *	
    *	@param startPos Posizione iniziale
    */
    GraphNode(Vector2i startPos);

	/** Distruttore. */
	~GraphNode();

	/** Copia le informazioni del nodo passato come parametro nel nodo corrente.
	*
	*	@param node Nodo sorgente.
	*/
	void copy(const GraphNode &node);

	/** Imposta l'adiacenza basandosi sulla direzione passata come parametro.
	*
	*	@param dir Direzione di adiacenza
	*	@param nodo Puntatore al nodo adiacente.
	*	@param peso Peso del collegamento.
	*	@return True se ha impostato l'adiacenza. False altrimenti.
	*/
	bool setAdiacenza(Direzione dir, GraphNode * nodo, int peso);

	/** Restituisce, se esiste, il nodo adiacente e il peso del collegamento.
	*
	*	@param i Posizione nell'array della adiacenza
	*	@param peso Parametro di uscita: peso del collegamento
	*	@return Puntatore al nodo adiacente. NULL se non esiste il nodo adiacente
	*/
	GraphNode * getAdiacenza(int i, int &peso);
	
	/** Restituisce, se esiste, il nodo adiacente e il peso del collegamento.
	*
	*	@param dir Direzione di adiacenza
	*	@param peso Parametro di uscita: peso del collegamento
	*	@return Puntatore al nodo adiacente. NULL se non esiste il nodo adiacente
	*/
	GraphNode * getAdiacenza(Direzione dir, int &peso);
};

#endif // !GRAPHNODE_INCLUDE