#include "../header/Core.h"

using namespace sf;

static Mappa mappa;

void CoreInit()
{
	mappa.generate();
}

Macchina m1 = Macchina(Vector2i(34, 3 * 68 - 20));
Macchina m2 = Macchina(Vector2i(0, 1 * 68 - 20));
Macchina m3 = Macchina(Vector2i(0, 5 * 68 - 20));




void updateMacchina(Macchina &macchina)
{
	macchina.update();
	Vector2f f = macchina.getShape().getPosition();
	Blocco * b = mappa.getBlocco(Vector2i(f.x, f.y)); //prendo il blocco su cui si trova la macchina
	if (b != NULL) {
		Curva * c = dynamic_cast<Curva *>(b);
		if (c != NULL)
			macchina.changeDirection(c->getChangeDir(macchina.getShape().getPosition())); //delego alla il cambio direzione della macchina
		Incrocio4 * i4 = dynamic_cast<Incrocio4 *>(b);
		if (i4 != NULL) {
			Direzionatore random = Direzionatore();
			macchina.changeDirection(i4->getChangeDir(macchina.getShape().getPosition(), Direzione::SX)); //delego all'incrocio instradare correttamente la macchina
		}
		if (isEmptyBlock(b->getTipo()))
			macchina.stop();
	}
	else
		macchina.stop();
}

void update(RenderWindow &widget)
{	
	mappa.draw(widget);
	if (Keyboard::isKeyPressed(Keyboard::R)) {
		mappa.generate();
		m1.setPosition(Vector2i(34, 3 * 68 - 20));
		m1.changeDirection(Direzione::DX);
	}
	
	m2.setColor(Color::Magenta);
	m3.setColor(Color::Green);
	m1.draw(widget);
	m2.draw(widget);
	m3.draw(widget);

	updateMacchina(m1);
	updateMacchina(m2);
	updateMacchina(m3);
}


