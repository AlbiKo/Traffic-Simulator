#include <SFML/Graphics.hpp>
#include "Blocco.h"
#include "Debug.h"

using namespace sf;

class Mappa
{
private:
	Texture texture[11];

	Blocco ** blocchi;
	void loadTextures();
public:
    Mappa();

    void draw(RenderWindow &widget);
};
