#ifndef DIREZIONE_LIST_INCLUDE
#define DIREZIONE_LIST_INCLUDE

#include "Direzione.h"

/** Struttura che rappresenta il singolo nodo della lista. */
struct Direzione_Node {
	Direzione node;
	Direzione_Node * next;
};

/** La classe Direzione_List rappresenta una lista di elementi di tipo ::Direzione.
*	La classe tiene traccia del numero di elementi presenti nella lista.
*
*	I metodi disponibili sono quelli basici delle liste: inserimento, estrazione, ricerca.
*/
class Direzione_List {

private:
	/** Puntatore alla radice della lista. */
	Direzione_Node *root;

	/** Numero di nodi della lista. */
	int num_nodi;

public:
	/** Costruttore che inizializza una lista vuota. */
	Direzione_List();

	/** Costruttore che inizializza una lista con un elemento passato per parametro.
	*
	*	@param r Elemento iniziale.
	*/
	Direzione_List(Direzione r);

	/** Distruttuore che elimina tutti i nodi presenti nella lista. */
	~Direzione_List();

	/** Restituisce il numero di elementi nella lista. */
	inline int count()
	{
		return num_nodi;
	}

	/** Inserisce un elemento nella lista.
	*
	*	@param r Elemento da inserire.
	*/
	void insert(Direzione r);

	/** Restituisce l'elemento corrispondente all'indice passato come parametro.
	*	È necessario indicare se l'elemento deve essere rimosso o mantenuto.
	*
	*	@param index Indice nella lista dell'elemento.
	*	@param del True se l'elemento deve essere cancellato. False altrimenti.
	*	@return Se l'elemento esiste nella lista, lo restituisce. Altrimenti restiuisce la direzione nulla.
	*/
	Direzione get(int index, bool del);

	/** Restituisce il primo elemento corrispondente ai valori passati come parametro.
	*	È necessario indicare se l'elemento deve essere rimosso o mantenuto.
	*
	*	@param d Direzione da cercare.
	*	@param del True se l'elemento deve essere cancellato. False altrimenti.
	*	@return Se l'elemento esiste nella lista, lo restituisce. Altrimenti restiuisce la direzione nulla.
	*/
	Direzione get(Direzione d, bool del);

	/** Svuota la lista, cancellando tutti gli elementi. */
	void clean();
};

#endif