#ifndef VECTOR2I_LIST_INCLUDE
#define VECTOR2I_LIST_INCLUDE


#include <SFML/Graphics.hpp>

using namespace sf;

/** Struttura che rappresenta il singolo nodo della lista */
struct Vector2i_Node {
	Vector2i node;
	Vector2i_Node * next;
};

/** La classe Vector2i_List rappresenta una lista di elementi di tipo Vector2i, ereditati da SFML.
*	Gli elementi Vector2i sono vettori che contengono 2 numeri interi.
*	La classe tiene traccia del numero di elementi presenti nella lista.
*
*	I metodi disponibili sono quelli basici delle liste: inserimento, estrazione, ricerca
*	@author Alberto Beccari
*/
class Vector2i_List {

private:
	/** Puntatore alla radice della lista */
	Vector2i_Node *root;

	/** Numero di nodi della lista */
	int num_nodi;

public:
	/** Costruttore che inizializza una lista vuota */
	Vector2i_List();

	/** Costruttore che inizializza una lista con un elemento passato per parametro
	*
	*	@param r Elemento iniziale
	*/
	Vector2i_List(Vector2i r);
	
	/** Distruttuore che elimina tutti i nodi presenti nella lista */
	~Vector2i_List();	
	
	/** Restituisce il numero di elementi nella lista */
	inline int count()
	{
		return num_nodi;
	}

	/** Inserimento di un elemento nella lista 
	*
	*	@param r Elemento da inserire
	*/
	void insert(Vector2i r);

	/** Restituisce l'elemento corrispondente all'indice passato come parametro. 
	*	È necessario indicare se l'elemento deve essere rimosso o mantenuto. 
	*
	*	@param index Indice nella lista dell'elemento
	*	@param del True se l'elemento deve essere cancellato. False altrimenti
	*	@return Se l'elemento esiste nella lista, lo restituisce. Altrimenti restiuisce un vettore (-1, -1)
	*/
	Vector2i get(int index, bool del);

	/** Restituisce il primo elemento corrispondente ai valori passati come parametro. 
	*	È necessario indicare se l'elemento deve essere rimosso o mantenuto.
	*
	*	@param x Valore x del vettore da cercare
	*	@param y Valore y del vettore da cercare
	*	@param del True se l'elemento deve essere cancellato. False altrimenti
	*	@return Se l'elemento esiste nella lista, lo restituisce. Altrimenti restiuisce un vettore (-1, -1)
	*/
	Vector2i get(int x, int y, bool del);

	/** Svuota la lista, cancellando tutti gli elementi. */
	void clean();

};

#endif
