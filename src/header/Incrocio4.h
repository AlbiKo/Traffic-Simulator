#include "Incroci.h"
#include <SFML/Graphics.hpp>
#include "Direzione.h"

using namespace sf;

class Incrocio4: public Incroci
{

public:
	Incrocio4() ;
	Incrocio4(int rowIndex, int columnIndex);
	RectangleShape shape_1, shape_2, shape_3, shape_4; //DEBUG
};
