#include "TipoBlocco.h"
#include "enum_toInt.h"
#include <SFML/Graphics.hpp>

#ifndef BLOCCO_INCLUDE
#define BLOCCO_INCLUDE
using namespace sf;

class Blocco
{
protected:
	Sprite sprite;
	TipoBlocco tipo;

	void setTipo(TipoBlocco tipo);
public:
	static const int size = 68;

	Blocco();
	virtual ~Blocco();
	Blocco(int rowIndex, int columnIndex, TipoBlocco tipo);

	inline Sprite getSprite()
	{
		return sprite;
	}

	inline TipoBlocco getTipo()
	{
		return tipo;
	}

	inline Vector2i coordBlocco()
	{
		return Vector2i(sprite.getPosition().x / size,sprite.getPosition().y / size);
	}

	virtual void cambiaVerso(TipoBlocco verso);

};

#endif
