#include "Blocco.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Incrocio4: public Blocco
{

private:
	Vector2i pos1, pos2, pos3, pos4;
public:
	Incrocio4() ;
	Incrocio4(int rowIndex, int columnIndex);
	RectangleShape shape_1, shape_2, shape_3, shape_4;

};
