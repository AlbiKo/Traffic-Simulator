#include "Incroci.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Incrocio3: public Incroci
{

public:
	Incrocio3() ;
	Incrocio3(int rowIndex, int columnIndex, TipoBlocco tipo);

	void cambiaVerso(TipoBlocco verso);
	RectangleShape shape_1, shape_2, shape_3, shape_4; //DEBUG
};
