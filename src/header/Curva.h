#include "Blocco.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Curva: public Blocco
{

private:

public:
	Curva() ;
	Curva(int rowIndex, int columnIndex, TipoBlocco tipo);

	void cambiaVerso(TipoBlocco verso);
};
