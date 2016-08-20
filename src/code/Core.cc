#include "../header/Core.h"

using namespace sf;

static Mappa mappa;
static Graph g;

Macchina_List carlist;
Vector2i_List sorg;
Vector2i_List percorso;
Vector2i currentBlock;

int NUM_MACCHINE = 100;
void CoreInit()
{
	mappa.generate();
	g.buildGraph(mappa);

	
	mappa.getSorgenti(sorg);
	for (int i = 0; i < NUM_MACCHINE; i++)
	{
		Vector2i start = sorg.get(rand() % sorg.count(), false);
		addCar(start);
		g.findPath(start, sorg.get(rand() % sorg.count(), false), carlist.get(0)->percorso);
		carlist.get(0)->currentBlock = carlist.get(0)->percorso.get(0, false);
	}
}



void update(RenderWindow &widget)
{
	mappa.draw(widget);
	if (Keyboard::isKeyPressed(Keyboard::R))
	{
		mappa.generate();
	}

	for (int i = 0; i < NUM_MACCHINE; i++)
	{
		carlist.get(i)->draw(widget);
		updateMacchina(*carlist.get(i));
	}

	

}

Direzione findDir(Vector2f carPos, Vector2i nextBlock)
{
	if (nextBlock == Vector2i(-1, -1))
		return Direzione::ND;

	Vector2i pos(static_cast<int>(carPos.x) / Blocco::size, static_cast<int>(carPos.y) / Blocco::size);

//	std::cerr << "nextB " << nextBlock.x << ", " << nextBlock.y 
	//			<<"curB " << pos.x << ", " << pos.y <<"\n";
	if (pos.y == nextBlock.y)
	{
		if (pos.x < nextBlock.x)
			return Direzione::DX;
		else
			return Direzione::SX;
	}

	if (pos.x == nextBlock.x)
	{
		if (pos.y < nextBlock.y)
			return Direzione::GIU;
		else
			return Direzione::SU;
	}

	//assert(0);
	return Direzione::ND;
}

void updateMacchina(Macchina &macchina)
{
	macchina.update();
	Vector2f f = macchina.getShape().getPosition();
	Blocco * b = mappa.getBlocco(Vector2i(f.x, f.y)); //prendo il blocco su cui si trova la macchina
	if (b != NULL) {
		if (static_cast<int>(f.x / Blocco::size) == macchina.currentBlock.x && static_cast<int>(f.y / Blocco::size) == macchina.currentBlock.y)
		{
			Vector2i t = macchina.percorso.get(0, false);
			if (t != Vector2i(-1,-1))
			std::cerr << "sono su " << t.x << ", " << t.y << " curBlock " 
				<< macchina.currentBlock.x << ", " << macchina.currentBlock.y <<"\n";
			macchina.percorso.get(0, true);
			macchina.currentBlock = macchina.percorso.get(0, false);
		}

		Curva * c = dynamic_cast<Curva *>(b);
		if (c != NULL)
			macchina.changeDirection(c->getChangeDir(macchina.getShape().getPosition())); //delego alla il cambio direzione della macchina
		Incroci * i = dynamic_cast<Incroci *>(b);
		if (i != NULL) {
			Direzione d = findDir(f, macchina.percorso.get(0, false));
			//std::cerr << "Direzione " << toString(d) << "\n";
			macchina.changeDirection(i->getChangeDir(macchina.getShape().getPosition(), d)); //delego all'incrocio instradare correttamente la macchina
		}
		if (isEmptyBlock(b->getTipo()))
		{
			//macchina.stop();
			percorso.clean();
			Vector2i start = sorg.get(rand() % sorg.count(), false);
			Direzione d;
			Vector2i temp = start;
			placeCar(temp, d);
			macchina.setPosition(temp);
			macchina.changeDirection(d);
			g.findPath(start, sorg.get(rand() % sorg.count(), false), macchina.percorso);
			macchina.currentBlock = macchina.percorso.get(0, false);
		}
			
	}
	else
	{
		percorso.clean();
		Vector2i start = sorg.get(rand() % sorg.count(), false);
		Direzione d;
		Vector2i temp = start;
		placeCar(temp, d);
		macchina.setPosition(temp);
		macchina.changeDirection(d);
		g.findPath(start, sorg.get(rand() % sorg.count(), false), macchina.percorso);
		macchina.currentBlock = macchina.percorso.get(0, false);
	}
}

void placeCar(Vector2i &source, Direzione &d)
{
	int x1, y1;
	d = Direzione::ND;
	if (source.x == 0 && source.y != 0) {
		d = Direzione::DX;
		x1 = 40;
		y1 = 48;
	}
	if (source.x == mappa.getMapSize().x - 1 && source.y != 0) {
		d = Direzione::SX;
		x1 = 40;
		y1 = 19;
	}
	if (source.x != 0 && source.y == 0) {
		d = Direzione::GIU;
		x1 = 19;
		y1 = 40;
	}
	if (source.x != 0 && source.y == mappa.getMapSize().y - 1) {
		d = Direzione::SU;
		x1 = 48;
		y1 = 40;
	}
	source.x = source.x * Blocco::size + x1;
	source.y = source.y * Blocco::size + y1;
}

void addCar(Vector2i source)
{
	Direzione d;
	placeCar(source, d);
	carlist.add(source, d);
}
