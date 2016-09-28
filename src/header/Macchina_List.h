#ifndef MACCHINA_LIST_INCLUDE
#define MACCHINA_LIST_INCLUDE

#include "Macchina.h"

/** Struttura che rappresenta il singolo nodo della lista */
struct Macchina_Node {
	Macchina node;
	Macchina_Node * next;
};

/** La classe Macchina_List rappresenta una lista di Macchine.
*	La classe tiene traccia del numero di elementi presenti nella lista.
*
*	I metodi disponibili sono: aggiunta di una nuova macchina, aggiunta di macchina esistente, svuotamento lista, restituzione di un elemento (senza rimuoverlo), rimozione di un singolo elemento.*/
class Macchina_List {

private:
	/** Puntatore alla radice della lista */
	Macchina_Node *root;

	/** Numero di nodi della lista */
	int num_nodi;

public:


	/** Costruttore che inizializza una lista vuota */
	Macchina_List();
	
	/** Distruttuore che elimina tutti i nodi presenti nella lista */
	~Macchina_List();	
	
	/** Restituisce il numero di elementi nella lista */
	inline int count()
	{
		return num_nodi;
	}

	/** Aggiunta di una macchina in testa alla lista
	*
	*	@param pos Posizione iniziale della macchina
	*	@param dir Direzione iniziale della macchina
	*	@return Puntatore della macchina inserita
	*/
	Macchina * add(Vector2i pos, Direzione dir);

	/**	Aggiunta di una macchina in testa alla lista
	*
	*	@param pos Posizione iniziale della macchina
	*	@param dir Direzione iniziale della macchina
	*	@param color Colore della macchina
	*	@return Puntatore della macchina inserita
	*/
	Macchina * add(Vector2i pos, Direzione dir, Color color);

	/** Inserimento di una macchina esistente nella lista 
	*
	*	@param i Macchina da inserire
	*/
	void insert(Macchina i);

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
};
#endif