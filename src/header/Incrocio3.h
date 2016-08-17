#include "Blocco.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Incrocio3: public Blocco
{

private:
	Vector2i pos1, pos2, pos3, pos4;
public:
	Incrocio3() ;
	Incrocio3(int rowIndex, int columnIndex, TipoBlocco tipo);

	void cambiaVerso(TipoBlocco verso);
	RectangleShape shape_1, shape_2, shape_3, shape_4;
};
