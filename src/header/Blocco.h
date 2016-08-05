#include "TipoBlocco.h"
#include "enum_toInt.h"
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

	/**Impsta il tipo di blocco con quello indicato*/
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

	/**Cambia il verso del blocco con quello indicato*/
	virtual void cambiaVerso(TipoBlocco verso);

};

#endif
