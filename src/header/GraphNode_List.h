#ifndef GRAPHNODE_LIST_INCLUDE
#define GRAPHNODE_LIST_INCLUDE

#include "GraphNode.h"

/** Struttura che rappresenta il singolo nodo della lista. */
struct GraphNode_Node {
	GraphNode node;
	GraphNode_Node * next;
};

/** La classe GraphNode_List rappresenta una lista di elementi di tipo ::GraphNode.
*	La classe tiene traccia del numero di elementi presenti nella lista.
*
*	I metodi disponibili sono quelli basici delle liste: inserimento, estrazione, ricerca.
*/
class GraphNode_List {

private:
	/** Puntatore alla radice della lista. */
	GraphNode_Node *root;

	/** Numero di nodi della lista. */
	int num_nodi;

public:
	/** Costruttore che inizializza una lista vuota. */
	GraphNode_List();

	/** Costruttore che inizializza una lista con un elemento passato per parametro.
	*
	*	@param r Elemento iniziale.
	*/
	GraphNode_List(GraphNode r);

	/** Distruttuore che elimina tutti i nodi presenti nella lista. */
	~GraphNode_List();

	/** Restituisce il numero di elementi nella lista. */
	inline int count()
	{
		return num_nodi;
	}

	/** Inserimento di un elemento nella lista.
	*
	*	@param node Elemento da inserire.
	*/
	void insert(const GraphNode &node);

	/** Restituisce l'elemento corrispondente all'indice passato come parametro.
	*	È necessario indicare se l'elemento deve essere rimosso o mantenuto.
	*
	*	@param index Indice nella lista dell'elemento.
	*	@param del True se l'elemento deve essere cancellato. False altrimenti.
	*	@return Se l'elemento esiste nella lista, lo restituisce. Altrimenti restiuisce un ::GraphNode base.
	*/
	GraphNode get(int index, bool del);

	/** Restituisce il puntatore all'elemento corrispondente all'indice passato come parametro.
	*
	*	@param index Indice nella lista dell'elemento.
	*	@return Se l'elemento esiste nella lista, restituisce il suo puntatore. Altrimenti restiuisce NULL.
	*/
	GraphNode * get(int index);

	/** Restituisce il primo elemento corrispondente ai valori passati come parametro.
	*	È necessario indicare se l'elemento deve essere rimosso o mantenuto.
	*
	*	@param x Coordinata x del nodo da cercare.
	*	@param y Coordinata y del nodo da cercare.
	*	@param del True se l'elemento deve essere cancellato. False altrimenti.
	*	@return Se l'elemento esiste nella lista, lo restituisce. Altrimenti restiuisce un ::GraphNode base.
	*/
	GraphNode get(int x, int y, bool del);

	/** Restituisce il puntaore al primo elemento corrispondente ai valori passati come parametro.
	*
	*	@param x Coordinata x del nodo da cercare.
	*	@param y Coordinata y del nodo da cercare.
	*	@return Se l'elemento esiste nella lista, restituisce il suo puntatore. Altrimenti restiuisce NULL.
	*/
	GraphNode * get(int x, int y);

	/** Trova e restituisce l'indice nella lista dell'elemento che ha coordinate uguali al parametro. 
	*
	*	@param pos Coordinate del nodo da trovare/
	*	@return Se esiste, l'indice dell'elemento nella lista. Altrimenti -1.
	*/
	int getIndex(Vector2i pos);

	/** Svuota la lista, cancellando tutti gli elementi. */
	void clean();
};

#endif