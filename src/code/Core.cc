#include "../header/Core.h"

using namespace sf;

static Mappa mappa;

void CoreInit()
{
	mappa.generate();
}

void update(RenderWindow &widget)
{
	mappa.draw(widget);
	if (Keyboard::isKeyPressed(Keyboard::R))
		mappa.generate();

}
