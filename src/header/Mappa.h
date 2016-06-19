#include <SFML/Graphics.hpp>
#include "Blocco.h"
#include "Rettilineo.h"
#include "Curva.h"
#include "Incrocio3.h"
#include "Incrocio4.h"

using namespace sf;

class Mappa
{
private:

	int blocchiX, blocchiY;

	Blocco *** blocchi;
	void loadTextures();
	void cambiaTipoBlocco(Blocco * &blocco, TipoBlocco tipo);
public:
    Mappa();

    void draw(RenderWindow &widget);
};
