#include "TipoBlocco.h"
#include "enum_toInt.h"
#include "MacchinaPtr_List.h"
#include <SFML/Graphics.hpp>

#ifndef BLOCCO_INCLUDE
#define BLOCCO_INCLUDE
using namespace sf;

/**La classe blocco rappresenta il blocco generico che deve essere posizionato sulla mappa.
*/
class Blocco
{
protected:

	Sprite sprite;
	TipoBlocco tipo;

	bool checkBlockCollision(Macchina &currentCar);
	bool checkSameDirCollision(Macchina &currentCar, Macchina &collidingCar);
	bool checkCurveCollison(Macchina &currentCar, Macchina &collidingCar);
	bool checkCrossCollision(Macchina &currentCar, Macchina &collidingCar);

	/**Imposta il tipo di blocco con quello indicato*/
	void setTipo(TipoBlocco tipo);
public:
	static const int size = 68;

	MacchinaPtr_List cars;
	IntRect collider;
	void checkCollision();
	bool canBeSpawned(Macchina & car);

	Blocco();
	virtual ~Blocco();
	Blocco(int rowIndex, int columnIndex, TipoBlocco tipo);

	inline Sprite getSprite()
	{
		return sprite;
	}

	/**Restituzione del tipo di blocco*/
	inline TipoBlocco getTipo()
	{
		return tipo;
	}

	/**Restituzione delle coordinate del blocco*/
	inline Vector2i coordBlocco()
	{
		return Vector2i(sprite.getPosition().x / size,sprite.getPosition().y / size);
	}

	inline Vector2i getPos()
	{
		return Vector2i(sprite.getPosition().x, sprite.getPosition().y);
	}

	/**Cambia il verso del blocco con quello indicato*/
	virtual void cambiaVerso(TipoBlocco verso);

	virtual void draw(RenderWindow &widget);
};

#endif