#include "../header/Core.h"

static bool premuto = false;

using namespace sf;

RectangleShape * rect;
RectangleShape * rect2;
Texture texture;

void CoreInit()
{
	assert(texture.loadFromFile("media/img/verticale.jpg"));
	texture.setSmooth(true);

	rect = new RectangleShape(Vector2f(68, 68));
    rect->setTexture(&texture);

    rect2 = new RectangleShape(Vector2f(68, 68));
    rect2->setTexture(&texture, true);
    rect2->setPosition(0, 68);
}


void update(RenderWindow &widget)
{
	if (!premuto && Keyboard::isKeyPressed(Keyboard::S))
    {
        std::cout <<"premo S \n";
        rect->move(0, 1);
        premuto = true;
    }
    else if (!Keyboard::isKeyPressed(Keyboard::S))
        premuto = false;

	widget.draw(*rect);
	widget.draw(*rect2);
}
