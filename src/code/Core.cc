#include "../header/Core.h"

using namespace sf;

static Mappa map;
static Graph graph;

static Macchina_List carList;
static Vector2i_List sources;

Vector2i mapSize;

int NUM_MACCHINE = 5;

extern int RESX, RESY;

int pause = false;

void CoreInit()
{
	map.generate();
	mapSize = map.getMapSize();
	graph.buildGraph(map);
	
	map.getSorgenti(sources);
	for (int i = 0; i < NUM_MACCHINE; i++)
	{
		Vector2i start = sources.get(rand() % sources.count(), false);
		addCar(start);
		graph.findPath(start, sources, carList.get(0)->percorso);
		carList.get(0)->nextBlock = carList.get(0)->percorso.get(0, false);
		carList.get(0)->currentBlock = carList.get(0)->percorso.get(0, false);
	}
}

void update(RenderWindow &widget)
{
	map.draw(widget);

	if (Keyboard::isKeyPressed(Keyboard::R))
		refreshMap();

	if (Keyboard::isKeyPressed(Keyboard::A))
		replaceCar(*carList.get(rand() % carList.count()));

	if (Keyboard::isKeyPressed(Keyboard::D))
		carList.get(0)->changeDirection(carList.get(0)->currentDir);

	if (Keyboard::isKeyPressed(Keyboard::S))
		carList.get(1)->stop();

	if (Keyboard::isKeyPressed(Keyboard::X))
		carList.get(0)->stop();

	if (Keyboard::isKeyPressed(Keyboard::P))
		pause = true;

	if (Keyboard::isKeyPressed(Keyboard::L))
		pause = false;

	if (Keyboard::isKeyPressed(Keyboard::U))
	{
		Vector2i start = sources.get(rand() % sources.count(), false);
		addCar(start);
		graph.findPath(start, sources, carList.get(0)->percorso);
		carList.get(0)->nextBlock = carList.get(0)->percorso.get(0, false);
		carList.get(0)->currentBlock = carList.get(0)->percorso.get(0, false);
		NUM_MACCHINE++;
	}

	
		for (int i = 0; i < NUM_MACCHINE; i++)
		{
			Macchina * car = carList.get(i);

			car->draw(widget);
			if (!pause)
				updateCar(*car);
		}
		if (!pause)
			map.checkCarCollision();

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

	return Direzione::ND;
}

void updateCar(Macchina &car)
{
	car.update();
	Vector2f f = car.getShape().getPosition();
	Blocco * b = map.getBlocco(Vector2i(f.x, f.y)); //prendo il blocco su cui si trova la macchina

	if (b != NULL)
	{
		if (static_cast<int>(f.x) / Blocco::size == car.nextBlock.x && static_cast<int>(f.y)  / Blocco::size== car.nextBlock.y)
		{
			
			Blocco * blocco = map.getBlocco(car.currentBlock.y, car.currentBlock.x);
			if (car.collider.intersects(blocco->collider))
			{
			/*	std::cerr << "cur " << car.currentBlock.x << ", " << car.currentBlock.y << "\n";
				std::cerr << "next " <<car.nextBlock.x <<", " <<car.nextBlock.y  <<"\n";
				std::cerr << blocco->collider.left << ", " << blocco->collider.top << "  " << blocco->getSprite().getPosition().x << ", " << blocco->getSprite().getPosition().y << "\n";
				std::cerr << car.getPosition().x << ", " << car.getPosition().y << "\n";
				*/
				//Aggiungo la macchina al blocco appena lasciato
				blocco->cars.insert(&car);
			}

			car.currentBlock = car.nextBlock;

			blocco = map.getBlocco(car.currentBlock.y, car.currentBlock.x);
			if (car.collider.intersects(blocco->collider))
				//Aggiungo la macchina al blocco corrente
				blocco->cars.insert(&car);

			//Trovo il prossimo nodo nel percorso se ho raggiunto il nodo attuale
			if (car.nextBlock == car.percorso.get(0, false) && car.percorso.count() > 1)
			{
				car.percorso.get(0, true);
				car.nextDir = findDir(f, car.percorso.get(0, false));
			}
		
			//Trovo il prossimo blocco
			switch (car.nextDir)
			{
			case Direzione::SU:
				car.nextBlock.y--;
				break;
			case Direzione::GIU:
				car.nextBlock.y++;
				break;
			case Direzione::DX:
				car.nextBlock.x++;
				break;
			case Direzione::SX:
				car.nextBlock.x--;
				break;
			default:
				break;

			}
		}

		Blocco * blocco = map.getBlocco(car.nextBlock.y, car.nextBlock.x);
		if (blocco != NULL)
			//Aggiungo la macchina al blocco successivo
			blocco->cars.insert(&car);

		Curva * c = dynamic_cast<Curva *>(b);
		if (c != NULL)
			car.changeDirection(c->getChangeDir(car.getShape().getPosition())); //delego alla il cambio direzione della macchina
		
		Incroci * i = dynamic_cast<Incroci *>(b);
		if (i != NULL)
			car.changeDirection(i->getChangeDir(car.getShape().getPosition(), car.nextDir)); //delego all'incrocio instradare correttamente la macchina
		
		if (isEmptyBlock(b->getTipo()))
			replaceCar(car);
	}
	else
		replaceCar(car);
}

void placeCar(Vector2i &source, Direzione &d)
{
	int x1, y1;
	d = Direzione::ND;
	if (source.x == 0 && source.y != 0) {
		d = Direzione::DX;
		x1 = 0;
		y1 = 48;
	}
	if (source.x == map.getMapSize().x - 1 && source.y != 0) {
		d = Direzione::SX;
		x1 = 66;
		y1 = 19;
	}
	if (source.x != 0 && source.y == 0) {
		d = Direzione::GIU;
		x1 = 19;
		y1 = 0;
	}
	if (source.x != 0 && source.y == map.getMapSize().y - 1) {
		d = Direzione::SU;
		x1 = 48;
		y1 = 66;
	}
	source.x = source.x * Blocco::size + x1;
	source.y = source.y * Blocco::size + y1;
}

void replaceCar(Macchina & car)
{
	Vector2i startSource = sources.get(rand() % sources.count(), false);
	Direzione startDir;
	Vector2i screenPos = startSource;

	placeCar(screenPos, startDir);
	car.setPosition(screenPos);
	car.changeDirection(startDir);

	graph.findPath(startSource, sources, car.percorso);
	car.nextBlock = car.percorso.get(0, false);
	car.currentBlock = car.percorso.get(0, false);
}

void refreshMap()
{
	map.generate();

	sources.clean();
	map.getSorgenti(sources);
	graph.buildGraph(map);

	for (int i = 0; i < NUM_MACCHINE; i++)
		replaceCar(*carList.get(i));
}

void addCar(Vector2i source)
{
	Direzione d;
	placeCar(source, d);
	carList.add(source, d);
}

