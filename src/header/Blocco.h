#include "TipoBlocco.h"
#include "Debug.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Blocco
{
private:
	Sprite sprite;
	TipoBlocco tipo;

public:
	static const int size = 68;

	Blocco(int x, int y, Texture &texture);

	inline Sprite getSprite()
	{
		return sprite;
	}

	inline TipoBlocco getTipo()
	{
		return tipo;
	}

	void setTipo(TipoBlocco tipo);


};
