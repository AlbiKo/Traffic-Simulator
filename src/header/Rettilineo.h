#include "Blocco.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Rettilineo: public Blocco
{
public:
	Rettilineo();
	Rettilineo(int rowIndex, int columnIndex, TipoBlocco tipo);

	void cambiaVerso(TipoBlocco verso);
	void draw(RenderWindow &widget);
};