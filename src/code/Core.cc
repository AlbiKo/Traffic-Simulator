#include "../header/Core.h"

using namespace sf;
/*
RectangleShape * rect;
RectangleShape * rect2;
Sprite * sprite;*/

Mappa mappa;

void CoreInit()
{
	//assert(texture.loadFromFile("media/img/verticale.jpg"));
	//texture.setSmooth(true);

	/*rect = new RectangleShape(Vector2f(68, 68));
    rect->setTexture(&texture);

    rect2 = new RectangleShape(Vector2f(68, 68));
    rect2->setTexture(&texture, true);
    rect2->setPosition(0, 68);

    sprite = new Sprite();
    sprite->setPosition(68, 0);
    sprite->setTexture(texture);*/
}



void update(RenderWindow &widget)
{
	mappa.draw(widget);
}
