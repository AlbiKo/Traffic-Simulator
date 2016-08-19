#ifndef GRAPHNODE_LIST_INCLUDE
#define GRAPHNODE_LIST_INCLUDE

#include "GraphNode.h"

/** Struttura che rappresenta il singolo nodo della lista */
struct GraphNode_Node {
	GraphNode node;
	GraphNode_Node * next;
};

/** La classe GraphNode_List rappresenta una lista di elementi di tipo ::Blocco.
*	La classe tiene traccia del numero di elementi presenti nella lista.
*
*	I metodi disponibili sono quelli basici delle liste: inserimento, estrazione, ricerca
*/
class GraphNode_List {

private:
	/** Puntatore alla radice della lista */
	GraphNode_Node *root;

	/** Numero di nodi della lista */
	int num_nodi;

public:
	/** Costruttore che inizializza una lista vuota */
	GraphNode_List();

	/** Costruttore che inizializza una lista con un elemento passato per parametro
	*
	*	@param r Elemento iniziale
	*/
	GraphNode_List(GraphNode r);

	/** Distruttuore che elimina tutti i nodi presenti nella lista */
	~GraphNode_List();

	/** Restituisce il numero di elementi nella lista */
	inline int count()
	{
		return num_nodi;
	}

	/** Inserimento di un elemento nella lista
	*
	*	@param r Elemento da inserire
	*/
	void insert(GraphNode r);

	/** Restituisce l'elemento corrispondente all'indice passato come parametro.
	*	È necessario indicare se l'elemento deve essere rimosso o mantenuto.
	*
	*	@param index Indice nella lista dell'elemento
	*	@param del True se l'elemento deve essere cancellato. False altrimenti
	*	@return Se l'elemento esiste nella lista, lo restituisce. Altrimenti restiuisce un blocco generico vuoto
	*/
	GraphNode get(int index, bool del);
	GraphNode * get(int index);
	/** Restituisce il primo elemento corrispondente ai valori passati come parametro.
	*	È necessario indicare se l'elemento deve essere rimosso o mantenuto.
	*
	*	@param x Coordinata x del blocco da cercare
	*	@param y Coordinata y del blocco da cercare
	*	@param del True se l'elemento deve essere cancellato. False altrimenti
	*	@return Se l'elemento esiste nella lista, lo restituisce. Altrimenti restiuisce un blocco generico vuoto
	*/
	GraphNode get(int x, int y, bool del);
	GraphNode * get(int x, int y);
	int getIndex(Vector2i pos);
	/** Svuota la lista, cancellando tutti gli elementi. */
	void clean();
};

#endif