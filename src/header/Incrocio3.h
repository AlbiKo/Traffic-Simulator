#include "Blocco.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Incrocio3: public Blocco
{

private:

public:
	Incrocio3() ;
	Incrocio3(int rowIndex, int columnIndex, TipoBlocco tipo);

	void cambiaVerso(TipoBlocco verso);
};
