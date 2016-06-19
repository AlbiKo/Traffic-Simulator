#include "Blocco.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Rettilineo: public Blocco
{

private:

public:
	Rettilineo();
	Rettilineo(int rowIndex, int columnIndex, TipoBlocco tipo);

	virtual void cambiaVerso(TipoBlocco verso);
};
