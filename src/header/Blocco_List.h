#ifndef BLOCCO_LIST_INCLUDE
#define BLOCCO_LIST_INCLUDE

#include "Blocco.h"

/** Struttura che rappresenta il singolo nodo della lista. */
struct Blocco_Node {
	Blocco node;
	Blocco_Node * next;
};

/** La classe Blocco_List rappresenta una lista di elementi di tipo ::Blocco.
*	La classe tiene traccia del numero di elementi presenti nella lista.
*
*	I metodi disponibili sono quelli basici delle liste: inserimento, estrazione, ricerca.
*/
class Blocco_List {

private:
	/** Puntatore alla radice della lista. */
	Blocco_Node *root;

	/** Numero di nodi della lista. */
	int num_nodi;

public:
	/** Costruttore che inizializza una lista vuota. */
	Blocco_List();

	/** Costruttore che inizializza una lista con un elemento passato per parametro.
	*
	*	@param r Elemento iniziale.
	*/
	Blocco_List(Blocco r);

	/** Distruttuore che elimina tutti i nodi presenti nella lista. */
	~Blocco_List();

	/** Restituisce il numero di elementi nella lista. */
	inline int count()
	{
		return num_nodi;
	}

	/** Inserisce un elemento nella lista.
	*
	*	@param r Elemento da inserire.
	*/
	void insert(Blocco r);

	/** Restituisce l'elemento corrispondente all'indice passato come parametro.
	*	È necessario indicare se l'elemento deve essere rimosso o mantenuto.
	*
	*	@param index Indice nella lista dell'elemento.
	*	@param del True se l'elemento deve essere cancellato. False altrimenti.
	*	@return Se l'elemento esiste nella lista, lo restituisce. Altrimenti restiuisce un blocco generico vuoto.
	*/
	Blocco get(int index, bool del);

	/** Restituisce il primo elemento corrispondente ai valori passati come parametro.
	*	È necessario indicare se l'elemento deve essere rimosso o mantenuto.
	*
	*	@param x Coordinata x del blocco da cercare.
	*	@param y Coordinata y del blocco da cercare.
	*	@param del True se l'elemento deve essere cancellato. False altrimenti.
	*	@return Se l'elemento esiste nella lista, lo restituisce. Altrimenti restiuisce un blocco generico vuoto.
	*/
	Blocco get(int x, int y, bool del);

	/** Svuota la lista, cancellando tutti gli elementi. */
	void clean();
};

#endif