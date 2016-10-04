#ifndef MACCHINAPTR_LIST_INCLUDE
#define MACCHINAPTR_LIST_INCLUDE

#include "Macchina.h"

/** Struttura che rappresenta il singolo nodo della lista. */
struct MacchinaPtr_Node {
	Macchina * node;
	MacchinaPtr_Node * next;
};


/** La classe MacchinaPtr_List rappresenta una lista di puntatori agli elementi di tipo ::Macchina.
*	La classe tiene traccia del numero di elementi presenti nella lista.
*
*	I metodi disponibili sono: aggiunta di un puntatore alla lista, svuotamento lista, restituzione di un elemento (senza rimuoverlo), rimozione di un singolo elemento.*/
class MacchinaPtr_List {

private:
	/** Puntatore alla radice della lista */
	MacchinaPtr_Node *root;

	/** Numero di nodi della lista */
	int num_nodi;

public:

	/** Costruttore che inizializza una lista vuota */
	MacchinaPtr_List();
	
	/** Distruttuore che elimina tutti i nodi presenti nella lista */
	~MacchinaPtr_List();	
	
	/** Restituisce il numero di elementi nella lista */
	inline int count()
	{
		return num_nodi;
	}


	/** Inserisce il puntatore di una macchina esistente nella lista 
	*
	*	@param i Macchina da inserire
	*/
	void insert(Macchina * i);

	/** Restituisce l'elemento corrispondente all'indice passato come parametro. 
	*
	*	@param index Indice nella lista dell'elemento
	*	@return Se l'elemento esiste nella lista, restituisce il suo puntatore. Altrimenti restiuisce un puntatore nullo
	*/
	Macchina * get(int index);


	/** Svuota la lista, cancellando tutti gli elementi. */
	void clean();

	/** Cancella l'elemento di indice i dalla lista
	*  @param i Posizione dell'elemento da cancellare
	*  @return true se la rimozione è avvenuta, false altrimenti.
	*/
	bool remove(int index);

	bool remove(Macchina * m);
};

#endif
