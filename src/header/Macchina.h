#ifndef MACCHINA_INCLUDE
#define MACCHINA_INCLUDE

#include <SFML/Graphics.hpp>
#include "Direzione.h"
#include "Vector2i_List.h"

using namespace sf;

/** La classe ::Macchina rappresenta il veicolo che viene posizionato sulla mappa.
*   Ogni macchina ha una sua posizione, un colore e la direzione in cui deve viaggiare.
*	Lo spostamento si basa sulla direzione corrente che si interrompe se viene rilevata 
*	una collisione con un altro veicolo mediante il collider che copre l'intera macchina e/o i collider a lato.
*/
class Macchina
{
public:
	/** Dimensione della macchina. */
	static const int SIZE = 15;

	/** Percorso che la macchina deve seguire. */
	Vector2i_List percorso;

	/** Coordinate del blocco su cui si trova la macchina. */
	Vector2i currentBlock;

	/** Coordinate del blocco da raggiungere. */
	Vector2i nextBlock;

	/** Direzione che dovrà intraprendere per raggiungere il prossimo blocco. */
	Direzione nextDir;

	/** Direzione corrente. */
	Direzione currentDir;

	/** Collider destro. */
	IntRect colliderDx,
			/** Collider sinistro. */
			colliderSx,
			/** Collider in alto. */
			colliderUp,
			/** Collider in basso. */
			colliderDown;

	/** Collider dello sprite. */
	IntRect collider;

#ifdef DEBUG_MODE
	/** Sprite per visualizzare i collider. */
	RectangleShape global, sx, dx, up, down;
#endif

	/** Se la macchina è disegnabile. */
	bool drawable = false;

	/** Puntatore alla macchina che ha fermato la macchina corrente. */
	Macchina * stoppedBy = NULL;

	/** Costruttore che inizializza una macchina senza specificare la posizione. */
	Macchina();
	
	/** Costruttore che inizializza una macchina alla posizione specificata.
	*
	*   @param pos Coordinate della posizione della macchina.
	*/
	Macchina(Vector2i pos);
	
	/** Distruttore. */
	~Macchina();

	/** Disegna sul widget la macchina. */
	void draw(RenderWindow &widget);

	/** Imposta la posizione della macchina.
	*
	*   @param pos Coordinate della posizione della macchina.
	*/
	void setPosition(Vector2i pos);

	/** Imposta il colore della macchina col colore passato come parametro.
	*
	*   @param c Nuovo colore della macchina.
	*/
	void setColor(Color c);

	/** Restituisce lo sprite della macchina. */
	RectangleShape getShape();

	/** Restituisce un Vector2i che contiene le coordinate. */
	inline Vector2i getPosition()
	{
		return Vector2i(getShape().getPosition().x, getShape().getPosition().y);
	}

	/** Restituisce il colore attuale della macchina. */
	Color getColor();

	/** Copia le informazioni della macchina passata come parametro nella macchina attuale.
	*
	*   @param m Macchina contenente le informazioni da copiare
	*/
	void copy(Macchina m);

	/** Indica se la macchina è ferma oppure no.
	*
	*	@return true se la macchina ha una direzione in cui muoversi (si sta muovendo), false se è ferma. 
	*/
	bool isMoving();

	/** Cambia la direzione in cui la macchina deve muoversi. Non permette di fermare la macchina.
	*
	*   @param dir Nuova direzione della macchina.
	*	@return true se la direzione è stata cambiata, false se la direzione non era valida.
	*/
	bool changeDirection(Direzione dir);

	/** Ferma la macchina. */
	void stop();

	/** Sposta la macchina nella direzione in cui deve andare. */
	void update();

	/** Restituisce la direzione della macchina. */
	Direzione getDirection();
	
private:

	/** Velocità base della macchina. */
	const int SPEED = 1;

	/** Sprite della macchina. */
	RectangleShape shape;

	/** Direzione della macchina. */
	Direzione moving;

	/** Inizializza lo sprite della macchina e i colliders. */
	void initMacchina();

	/** Aggiorna la posizione dei colliders. */
	void updateColliders();
};

#endif