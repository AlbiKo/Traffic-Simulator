#include "../header/Core.h"

using namespace sf;

static Mappa mappa;
static Graph g;

void CoreInit()
{
	mappa.generate();
	g.buildGraph(mappa);

	Vector2i_List sorg;
	mappa.getSorgenti(sorg);
	Vector2i_List percorso;
	g.findPath(sorg.get(rand() % sorg.count(), false), sorg.get(rand() % sorg.count(), false), percorso);
	D1(PRINT(" perco: " << percorso.get(0, false).x << ", " << percorso.get(0, false).y));
	D1(PRINT(" perco: " << percorso.get(1, false).x << ", " << percorso.get(1, false).y));
}

Macchina m = Macchina(Vector2i(34, 2 * 68 - 20));
Macchina m1 = Macchina(Vector2i(34, 3 * 68 - 20));

void update(RenderWindow &widget)
{
	
	mappa.draw(widget);
	if (Keyboard::isKeyPressed(Keyboard::R))
		mappa.generate();
	
	m.draw(widget);
	m1.draw(widget);
	m.update();
	m1.update();
	
	int x = m.getShape().getPosition().x; 
	m.setPosition(Vector2i(x % 1000, m.getShape().getPosition().y));

	x = m1.getShape().getPosition().x;
	m1.setPosition(Vector2i(x % 1000, m1.getShape().getPosition().y));

	
}
