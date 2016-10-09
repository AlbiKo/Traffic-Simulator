#ifndef GRAPH_INCLUDE
#define GRAPH_INCLUDE

#include "GraphNode.h"
#include "GraphNode_List.h"
#include "Mappa.h"

/** La classe ::Graph rappresenta il grafo costruito basandosi sulla mappa e usato per
*	calcolare il percorso da far seguire alle macchine.
*	Il grafo è composto da nodi di tipo ::GraphNode e, riferendosi alla mappa, un nodo può essere
*	soltanto una sorgente, una curva o un incrocio. Quindi i rettilinei non vengono considerati nodi,
*	ma vengono sottointesi.
*/
class Graph
{
public:
	/** Costruttore base. */
	Graph();

	/** Distruttore. */
	~Graph();

	/** Si costruisce il grafo prendendo come riferimento la mappa passata come parametro.
	*	Come nodi, vengono considarati solo i nodi sorgente e i nodi di tipo curva e incrocio.
	*
	*	@param map Mappa su cui basare la costruzione del grafo.
	*/
	void buildGraph(Mappa &map);

	/** Trova il percorso fra la sorgente di partenza e una scelta casualmente dalla
	*	lista di sorgenti raggiungibili.
	*
	*	@param startPos Posizione iniziale.
	*	@param sources Lista delle sorgenti.
	*	@param path Lista su cui verrà salvato il percorso.
	*/
	void findPath(Vector2i startPos, Vector2i_List &sources, Vector2i_List &path);
private:
	/** Lista dei nodi del grafo. */
	GraphNode_List nodes;

	/** Dimensioni della mappa. */
	Vector2i mapSize;

	/** Pesi costanti relativi alle categorie di blocco. */
	const int	RECT_WEIGHT		= 1,
				CURVE_WEIGHT	= 2,
				CROSS3_WEIGHT	= 3,
				CROSS4_WEIGHT	= 4;

	/** Collega i due nodi in entrambi i versi calcolando il peso del collegamento, rettilinei inclusi.
	*
	* @param currentNode Nodo da dove parte il collegamento.
	* @param nextNode Nodo a cui ci si deve collegare.
	* @param map Mappa su cui basare la costruzione del grafo.
	* @param numRect Numero di rettilinei fra le due posizioni.
	*/
	void addLink(GraphNode &currentNode, GraphNode &nextNode,  Mappa &map, int numRect);

	/** Controlla se il tipo di blocco attuale corrisponde a uno dei tre tipi di blocco 
	*	specificati come parametri o ad un incrocio a quattro.
	*
	*	@param blocco Tipo di blocco attuale.
	*	@param curva1 Primo tipo di curva.
	*	@param curva2 Secondo tipo di curva.
	*	@param cross3 Tipo di incrocio a tre.
	*	@return True se il blocco corrisponde a uno dei tre parametri o ad un incrocio a quattro. False altrimenti.
	*/
	bool checkLinkable(TipoBlocco blocco, TipoBlocco curva1, TipoBlocco curva2, TipoBlocco cross3);

	/** Individua, se il tipo di blocco attuale lo permette, il primo nodo compatibile a sinistra con il nodo attuale.
	*	Il nodo attuale e il nodo individuato vengono collegato fra loro.
	*
	*	@param currentPtr Puntatore al nodo corrente.
	*	@param tipo	Tipo del blocco corrente.
	*	@param map Mappa su cui basare la costruzione del grafo.
	*/
	void checkLinkSx(GraphNode * currentPtr, TipoBlocco tipo, Mappa &map);

	/** Individua, se il tipo di blocco attuale lo permette, il primo nodo compatibile a destra con il nodo attuale.
	*	Il nodo attuale e il nodo individuato vengono collegato fra loro.
	*
	*	@param currentPtr Puntatore al nodo corrente.
	*	@param tipo	Tipo del blocco corrente.
	*	@param map Mappa su cui basare la costruzione del grafo.
	*/
	void checkLinkDx(GraphNode * currentPtr, TipoBlocco tipo, Mappa &map);

	/** Individua, se il tipo di blocco attuale lo permette, il primo nodo compatibile in alto con il nodo attuale.
	*	Il nodo attuale e il nodo individuato vengono collegato fra loro.
	*
	*	@param currentPtr Puntatore al nodo corrente.
	*	@param tipo	Tipo del blocco corrente.
	*	@param map Mappa su cui basare la costruzione del grafo.
	*/
	void checkLinkUp(GraphNode * currentPtr, TipoBlocco tipo, Mappa &map);

	/** Individua, se il tipo di blocco attuale lo permette, il primo nodo compatibile in basso con il nodo attuale.
	*	Il nodo attuale e il nodo individuato vengono collegato fra loro.
	*
	*	@param currentPtr Puntatore al nodo corrente.
	*	@param tipo	Tipo del blocco corrente.
	*	@param map Mappa su cui basare la costruzione del grafo.
	*/
	void checkLinkDown(GraphNode * currentPtr, TipoBlocco tipo, Mappa &map);

	/** Basandosi sul tipo di blocco passato come parametro, ne restituisce il suo peso.
	*
	*	@param tipo Tipo del blocco.
	*	@return Peso del blocco.
	*/
	int getWeight(TipoBlocco tipo);

	/** Costruisce il percorso iniziando dalla destinazione e percorrendo verso la partenza i parent.
	*
	*	@param path Parametro di uscita: lista su cui salvare il percorso costruito.
	*	@param parent Array dei parent. Gli indici degli elementi nell'array corrispondono agli indici degli elementi nella lista.
	*	@param count Dimensione dell'array dei parent.
	*	@param endPos Posizione della destinazione.
	*/
	void buildPath(Vector2i_List &path, Vector2i parent[], int count, Vector2i endPos);

	/** Costruisce l'array dei parent e l'array delle distanze. 
	*
	*	@param parent Array dei parent. Gli indici degli elementi nell'array corrispondono agli indici degli elementi nella lista.
	*	@param dist Array delle distanze. Gli indici degli elementi nell'array corrispondono agli indici degli elementi nella lista.
	*	@param count Dimensione degli array parent e dist.
	*	@param endPos Posizione della destinazione.
	*/
	void buildParentArray(Vector2i parent[], int dist[], int count, Vector2i startPos);

	/** Inserisce nello stack gli elementi che hanno peso diverso da -1 in ordine decrescente.
	*
	*	@param stack Stack sul quale si sta lavorando.
	*	@param adj Array per le posizioni dei nodi adiacenti; di valore (-1, -1) se non esiste l'adiacenza o il nodo è già stato visitato. L'array è di dimensione 4 per via delle quattro possibili direzioni di adiacenza.
	*	@param weight Array per i pesi del collegamento con i adiacenti; di valore -1 se non esiste l'adiacenza o il nodo è già stato visitato. L'array è di dimensione 4 per via delle quattro possibili direzioni di adiacenza.
	*/
	void insertStack(Vector2i_List &stack, Vector2i adj[], int weight[]);

	/** Trova l'elemento con il valore massimo diverso da 0 nell'array e ne restituisce l'indice. 
	*
	*	@param weight Array per i pesi del collegamento con i adiacenti; di valore -1 se non esiste l'adiacenza o il nodo è già stato visitato. L'array è di dimensione 4 per via delle quattro possibili direzioni di adiacenza.
	*	@return L'indice del valore massimo se diverso da 0. Altrimenti -1.
	*/
	int findMaxAdj(int weight[]);
};

#endif // !GRAPH_INCLUDE