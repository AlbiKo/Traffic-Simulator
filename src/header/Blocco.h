#include "TipoBlocco.h"
#include "enum_toInt.h"
#include "MacchinaPtr_List.h"
#include <SFML/Graphics.hpp>

#ifndef BLOCCO_INCLUDE
#define BLOCCO_INCLUDE
using namespace sf;

/** La classe ::Blocco rappresenta il blocco generico che deve essere posizionato sulla mappa. */
class Blocco
{
protected:
	/** Sprite del blocco. Memorizza texture, posizione e grandezza. */
	Sprite sprite;

	/** Tipo del blocco (::TipoBlocco). */
	TipoBlocco tipo;

	/** Offset per il cambio direzione del veicolo. */
	static const int offsetChangeDirPos = 19;

	/** Controlla se uno dei collider della macchina (quello dello sprite e quelli a lato)
	*	collide con il collider del blocco.
	*
	*	@param currentCar Macchina.
	*	@return True se avviene la collisione. False altrimenti
	*/
	bool checkBlockCollision(Macchina &currentCar);

	/** Controlla se i collider degli sprite delle due macchine, le quali devono procedere
	*	verso la stessa direzione, collidono. Viene considerata solo la collisione in cui la
	*	macchina corrente si trova "dietro" alla macchina che si sta considerando per la
	*	collisione.
	*
	*	@param currentCar Macchina corrente.
	*	@param collidingCar Macchina con cui currentCar potrebbe collidere.
	*	@return True se avviene la collsione. False altrimenti.
	*/
	bool checkSameDirCollision(Macchina &currentCar, Macchina &collidingCar);

	/** Controlla se i collider degli sprite delle due macchine, in cui la seconda procede 
	*	verso una direzione perpendicolare a quella della prima, collidono. 
	*	Viene considerata solo la collisione in cui la macchina corrente si trova "dietro" 
	*	alla macchina che si sta considerando per la collisione.
	*
	*	@param currentCar Macchina corrente.
	*	@param collidingCar Macchina con cui currentCar potrebbe collidere.
	*	@return True se avviene la collsione. False altrimenti.
	*/
	bool checkCurveCollison(Macchina &currentCar, Macchina &collidingCar);

	/** Controlla se il collider al lato corrispondente alla direzione della prima macchina
	*	collide con il collider dello sprite della seconda.
	*	Viene considerata solo la collisione in cui la macchina corrente si trova "dietro"
	*	alla macchina che si sta considerando per la collisione.
	*
	*	@param currentCar Macchina corrente.
	*	@param collidingCar Macchina con cui currentCar potrebbe collidere.
	*	@return True se avviene la collsione. False altrimenti.
	*/
	bool checkCrossCollision(Macchina &currentCar, Macchina &collidingCar);

	/** Imposta il tipo di blocco con quello passato come parametro.
	*
	*	@param tipo Tipo di blocco da impostare.
	*/
	void setTipo(TipoBlocco tipo);

public:

	/** Dimensione in pixel del blocco. */
	static const int size = 68;

	/** Lista di puntatori a ::Macchina relativi alle macchine spawnate e attualmente
	*	in intersezione con il blocco.
	*/
	MacchinaPtr_List cars;

	/** Collider del blocco. */
	IntRect collider;
	
	/** Imposta un blocco di tipo ::TipoBlocco::EMPTY su (0,0). */
	Blocco();

	/** Imposta un blocco basandosi sui dati passati.
	*
	*	@param rowIndex Indice di riga nella matrice.
	*	@param columnIndex Indice di colonno nella matrice.
	*	@param tipo Tipo del blocco.
	*/
	Blocco(int rowIndex, int columnIndex, TipoBlocco tipo);

	/** Distruttore. */
	virtual ~Blocco();

	/** Restituisce lo sprite del blocco. */
	inline Sprite getSprite()
	{
		return sprite;
	}

	/** Restituisce il tipo di blocco. */
	inline TipoBlocco getTipo()
	{
		return tipo;
	}

	/** Restituisce le coordinate del blocco. 
	*
	*	@return Vector2i rappresentante le coordinate x,y del blocco.
	*/
	inline Vector2i coordBlocco()
	{
		return Vector2i(sprite.getPosition().x / size, sprite.getPosition().y / size);
	}

	/** Restituisce la posizione del blocco sulla finestra.
	*
	*	@return Vector2i rappresentante la posizione sulla finestra del blocco.
	*/
	inline Vector2i getPos()
	{
		return Vector2i(sprite.getPosition().x, sprite.getPosition().y);
	}

	/** Cambia il verso del blocco con quello passato come parametro. */
	virtual void cambiaVerso(TipoBlocco verso);

	/** Disegna il blocco. 
	*	Se il debug è attivo disegna anche i punti di cambio direzione.
	*
	*	@param widget Finestra su cui viene disegnato il blocco.
	*/
	virtual void draw(RenderWindow &widget);

	/** Controlla se ci sono collisioni fra le macchine presenti in lista, fermandole se necessario.
	*	Si controlla anche se una certa macchina si trova ancora nel blocco o nelle vicinanze,
	*	cancellandola dalla lista in caso negativo.
	*/
	void checkCollision();

	/** Controlla se la macchina può essere spawnata al bordo del blocco, quindi se il bordo
	*	non è occupata da nessun'altra macchina. 
	*
	*	@param car Macchina da spawnare.
	*	@return True se può essere spawnata. False altrimenti.
	*/
	bool canBeSpawned(Macchina & car);
};

#endif