#include "Incroci.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Incrocio3: public Incroci
{

private:

	void setWrongDir();

public:
	Incrocio3();
	Incrocio3(int rowIndex, int columnIndex, TipoBlocco tipo);

	void cambiaVerso(TipoBlocco verso);
	void draw(RenderWindow &widget);
};