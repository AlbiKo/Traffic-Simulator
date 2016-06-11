#include <SFML/Graphics.hpp>

using namespace sf;

/** La classe nodo rappresenta l'elemento base del grafo
*	utilizzato per calcolare il percorso delle macchine e
*	per capire come si dovrà disegnare la mappa.
*
*	Per quest'ultima cosa, tornerà utile il numero di adiacenze che,
*	in base al valore, si capirà che tipo di strada si dovrà disegnare.\n
*	Per esempio, se il numero adiacenze valesse 1 si avrebbe un rettilineo
*	Invece, se valesse 4 si avrebbe un incrocio.
*	@author Quello bravo
*/

class Nodo
{

private:

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
		Nodo * nodo;
		int peso;
	} adiacenze[4];

    int numAdiacenze;

	/**	Imposta e memorizza un'adiacenza con un certo nodo nell'apposito array.
	*	Viene aggiornato il peso, calcolato come distanza in pixel fra i due nodi.
	*	@param i Posizione dell'array in cui verrà memorizzata l'adiacenza
	*	@param nodo Puntatore al nodo adiacente.
	*/
	void setAdiacenza(int i, Nodo * nodo);

public:
	/** Costruttore base.  */
    Nodo();

    /** Costruttore che imposta la posizione di partenza  del nodo.
    *	Richiama il costruttore base.
    *	@param startPos Posizione iniziale
    */
    Nodo(Vector2i startPos);

	/** Cambia la posizione del nodo.
    *	Richiama il costruttore base.
    *	@param newPos Nuova posizione
    */
	void setPos(Vector2i newPos);

	/** Restituisce, se esiste, il nodo adiacente e il peso del collegamento.
    *	@param i Posizione nell'array delle adiacenze
    *	@param peso Parametro di uscita: peso del collegamento
	*	@return Puntatore al nodo adiacente
    */
	Nodo * getAdiacenza(int i, int &peso);

	/** Imposta alla prima posizione libera la nuova adiacenza e
	*	il peso del collegamento.
	*	@param nodo Puntatore al nodo adiacente
	*	@return True se ha trovato una posizione libera e inserito l'adiacenza.\n
	*			False in caso contrario.
	*/
	bool setAdiacenza(Nodo * nodo);

	inline int getNumAdiacenze()
	{
		return numAdiacenze;
	}

	inline Vector2i getPos()
	{
		return pos;
	}
};
