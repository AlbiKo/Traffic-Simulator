#ifndef MACCHINA_INCLUDE
#define MACCHINA_INCLUDE

#include <SFML/Graphics.hpp>
#include "Direzione.h"
#include "Blocco_List.h"

using namespace sf;

/** La classe Macchina rappresenta il veicolo che viene posizionato sulla mappa.
*   Ogni macchina ha una sua posizione, un colore e la direzione in cui deve viaggiare.
*/
class Macchina
{
public:
	/** Dimensione della macchina. */
	const int SIZE = 15;

	IntRect colliderDx,
			colliderSx,
			colliderUp,
			colliderDown;

	
	/** Costruttore che inizializza una macchina senza specificare la posizione. */
	Macchina();
	
	/** Costruttore che inizializza una macchina alla posizione specificata.
	*   @param pos Coordinate della posizione della macchina.
	*/
	Macchina(Vector2i pos);
	
	~Macchina();

	/** Disegna sul widget la macchina. */
	void draw(RenderWindow &widget);

	/** Imposta la posizione della macchina.
	*   @param pos Coordinate della posizione della macchina.
	*/
	void setPosition(Vector2i pos);

	/** Imposta il colore della macchina col colore passato come parametro.
	*   @param c Nuovo colore della macchina.
	*/
	void setColor(Color c);
	RectangleShape getShape();

	/** Restituisce un Vector2i che contiene le coordinate. */
	inline Vector2i getPosition() {
		return Vector2i(getShape().getPosition().x, getShape().getPosition().y);
	}

	/** Restituisce il colore attuale della macchina */
	Color getColor();

	/** Copia le informazioni della macchina passata come parametro nella macchina attuale.
	*   @param m Macchina contenente le informazioni da copiare
	*/
	void copy(Macchina m);

	/** Indica se la macchina è ferma oppure no.
	@return true se la macchina ha una direzione in cui muoversi (si sta muovendo), false se è ferma. */
	bool isMoving();

	/** Cambia la direzione in cui la macchina deve muoversi. Non permette di fermare la macchina.
	*   @param dir Nuova direzione della macchina
	*	@return true se la direzione è stata cambiata, false se la direzione non era valida.
	*/
	bool changeDirection(Direzione dir);

	/** Ferma la macchina. */
	void stop();

	/** Sposta la macchina nella direzione in cui deve andare. */
	void update();

	/** Restituisce la direzione della macchina */
	Direzione getDirection();
	
private:
	RectangleShape shape;

	/** Percorso che la macchina deve seguire. */
	Blocco_List percorso;
	
	/** Direzione della macchina. */
	Direzione moving;

	/** Inizializza lo sprite della macchina e i colliders. */
	void initMacchina();

	/** Aggiorna la posizione dei colliders. */
	void updateColliders();

};

#endif
