#include "TipoBlocco.h"

#include <SFML/Graphics.hpp>

using namespace sf;

class Blocco
{
private:
	Sprite sprite;
	TipoBlocco tipo;

public:
	static const int size = 68;

	Blocco(int rowIndex, int columnIndex, TipoBlocco tipo,  Texture * texture);

	inline Sprite getSprite()
	{
		return sprite;
	}

	inline TipoBlocco getTipo()
	{
		return tipo;
	}

	void setTipo(TipoBlocco tipo, Texture * texture);
};
