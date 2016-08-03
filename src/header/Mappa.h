#include <SFML/Graphics.hpp>
#include "TipoBlocco.h"
#include "enum_toInt.h"
#include "Blocco.h"
#include "Rettilineo.h"
#include "Curva.h"
#include "Incrocio3.h"
#include "Incrocio4.h"
#include "Vector2i_List.h"

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
    void generate();
};
