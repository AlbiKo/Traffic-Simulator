#include "../header/Core.h"

using namespace sf;

static Mappa mappa;
Macchina * m;
bool no = false;

Direzionatore dd;
Direzione dir = dd.estraiDirezione();

void CoreInit()
{
	mappa.generate();
	Vector2i a = mappa.getRandomSource();
	D1(PRINT("x: " << a.x << " - y: " << a.y << "\n"));
	int x1, y1;
	Direzione d = Direzione::ND;
	if (a.x == 0 && a.y != 0) {
		d = Direzione::DX;
		x1 = 40;
		y1 = 48;
		D1(PRINT("DX"));
	}
	if (a.x == 18 && a.y != 0) {
		d = Direzione::SX;
		D1(PRINT("SX"));
		x1 = 40;
		y1 = 19;
	}
	if (a.x != 0 && a.y == 0) {
		d = Direzione::GIU;
		D1(PRINT("GIU"));
		x1 = 19;
		y1 = 40;
	}
	if (a.x != 0 && a.y == 10) {
		d = Direzione::SU;
		D1(PRINT("SU"));
		x1 = 48;
		y1 = 40;
	}
	a.x = a.x * 68 + x1;
	a.y = a.y * 68 + y1;

	
	m = new Macchina(a);
	m->changeDirection(d);
}

/*Macchina m1 = Macchina(Vector2i(34, 3 * 68 - 20));
Macchina m2 = Macchina(Vector2i(0, 1 * 68 - 20));
Macchina m3 = Macchina(Vector2i(0, 5 * 68 - 20));*/




void updateMacchina(Macchina &macchina)
{
	macchina.update();
	Vector2f f = macchina.getShape().getPosition();
	Blocco * b = mappa.getBlocco(Vector2i(f.x, f.y)); //prendo il blocco su cui si trova la macchina
	if (b != NULL) {
		Curva * c = dynamic_cast<Curva *>(b);
		if (c != NULL)
			macchina.changeDirection(c->getChangeDir(macchina.getShape().getPosition())); //delego alla il cambio direzione della macchina
		Incroci * i = dynamic_cast<Incroci *>(b);
		if (i != NULL) {
			no = true;
			macchina.changeDirection(i->getChangeDir(macchina.getShape().getPosition(), dir)); //delego all'incrocio instradare correttamente la macchina
		}
		else {
			no = false;
			dd.ripristina();
			dd.escludiDirezione(getDirOpposta(macchina.getDirection()));
			dir = dd.estraiDirezione();
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
		CoreInit();
		//m1.setPosition(Vector2i(34, 3 * 68 - 20));
		//m1.changeDirection(Direzione::DX);
	}
	m->setColor(Color::Yellow);
	/*m2.setColor(Color::Magenta);
	m3.setColor(Color::Green);
	m1.draw(widget);
	m2.draw(widget);
	m3.draw(widget);*/
	m->draw(widget);

	/*updateMacchina(m1);
	updateMacchina(m2);
	updateMacchina(m3);*/
	updateMacchina(*m);
}


