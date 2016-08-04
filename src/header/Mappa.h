#include <SFML/Graphics.hpp>
#include "TipoBlocco.h"
#include "enum_toInt.h"
#include "Blocco.h"
#include "Rettilineo.h"
#include "Curva.h"
#include "Incrocio3.h"
#include "Incrocio4.h"
#include "Vector2i_List.h"
#include <ctime>

using namespace sf;

class Mappa
{
private:

	int blocchiX, blocchiY;
	Blocco *** blocchi;

	void loadTextures();
	void cambiaTipoBlocco(Blocco * &blocco, TipoBlocco tipo);

	void generateSource(int x, int y, bool vertical);
	void nome_casuale(int x, int y);
	void generateDX();
	void generateUP();
	void generateDOWN();
	bool randomBool();
	
public:
    Mappa();

    void draw(RenderWindow &widget);
    void generate();
};
