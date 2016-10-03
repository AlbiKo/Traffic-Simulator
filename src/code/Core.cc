#include "../header/Core.h"

using namespace sf;

static Mappa map;
static Graph graph;

static Macchina_List carList;
static Vector2i_List sources;
static MacchinaPtr_List pendingCarList;

Vector2i mapSize;

Clock clocks;

int NUM_MACCHINE = 100;
int spawned = 0;
Time timeLastSpawned;
Time timeLastSemUpdate;

extern int RESX, RESY;
#ifdef DEBUG_MODE
extern unsigned int MASK;
#endif // DEBUG_MODE

int pause = false;

void CoreInit()
{
	map.generate();
	mapSize = map.getMapSize();
	graph.buildGraph(map);
	
	map.getSorgenti(sources);
}

void update(RenderWindow &widget)
{
	map.draw(widget);

	Time elapsedTime = clocks.restart();
	timeLastSpawned += elapsedTime;
	timeLastSemUpdate += elapsedTime;

	if (!pause && timeLastSpawned.asSeconds() >= 0.25)
	{
		if (spawned < NUM_MACCHINE)
			createCar();

		
		timeLastSpawned = Time::Zero;
	}

	if (!pause && timeLastSemUpdate.asSeconds() >= 10)
	{
		map.updateSemaphores();
		timeLastSemUpdate = Time::Zero;
	}

	inputHandling(widget);
	
	for (int i = 0; i < spawned; i++)
	{
		Macchina * car = carList.get(i);

		if (car->drawable)
		{
			car->draw(widget);
			if (!pause)
				updateCar(*car);
		}	
	}

	for (int i = 0; i < pendingCarList.count(); i++)
	{
		Macchina * ptr = pendingCarList.get(i);
		Blocco * b = map.getBlocco(ptr->getPosition());
		if (b != NULL && b->canBeSpawned(*ptr))
		{
			ptr->drawable = true;
			ptr->changeDirection(ptr->currentDir);
			pendingCarList.remove(ptr);
			b->cars.insert(ptr);
			i--;
		}
	}

	if (!pause)
		map.checkCarCollision();

}

Direzione findDir(Vector2f carPos, Vector2i nextBlock)
{
	if (nextBlock == Vector2i(-1, -1))
		return Direzione::ND;

	Vector2i pos(static_cast<int>(carPos.x) / Blocco::size, static_cast<int>(carPos.y) / Blocco::size);

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
			car.currentBlock = car.nextBlock;

			Blocco * blocco = map.getBlocco(car.currentBlock.y, car.currentBlock.x);
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
		
		Incroci * i = dynamic_cast<Incroci *>(blocco);
		if (i != NULL)
		{
			IntRect sem = i->getSemaphore(car.currentDir);
			if (car.collider.intersects(sem))
				car.stop();
			else
				car.changeDirection(car.currentDir);
		}

		Curva * c = dynamic_cast<Curva *>(b);
		if (c != NULL)
			car.changeDirection(c->getChangeDir(car.getShape().getPosition())); //delego alla il cambio direzione della macchina
		
		i = dynamic_cast<Incroci *>(b);
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
	int x1 = 0, y1 = 0;
	d = Direzione::ND;

	if (source.x == 0 && source.y != 0)
	{
		d = Direzione::DX;
		x1 = 0;
		y1 = 48;
	}

	if (source.y != 0 && source.x == map.getMapSize().x - 1)
	{
		d = Direzione::SX;
		x1 = 66;
		y1 = 19;
	}

	if (source.x != 0 && source.y == 0)
	{
		d = Direzione::GIU;
		x1 = 19;
		y1 = 0;
	}

	if (source.x != 0 && source.y == map.getMapSize().y - 1)
	{
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

	pendingCarList.insert(&car);
	car.drawable = false;
	car.stop();
	car.stoppedBy = NULL;
	car.update();
	Blocco * b = map.getBlocco(car.getPosition());
	if (b != NULL)
		b->cars.insert(&car);


	graph.findPath(startSource, sources, car.percorso);
	car.nextBlock = car.percorso.get(0, false);
	car.currentBlock = car.nextBlock;
}

void refreshMap()
{
	map.generate();

	sources.clean();
	map.getSorgenti(sources);
	graph.buildGraph(map);
	pendingCarList.clean();

	for (int i = 0; i < spawned; i++)
		replaceCar(*carList.get(i));
}

void createCar()
{
	std::cerr << "Creo\n";
	Vector2i start = sources.get(rand() % sources.count(), false);
	Vector2i screenPos = start;
	Direzione d;
	placeCar(screenPos, d);
	Macchina * ptr = carList.add(screenPos, d);

	pendingCarList.insert(ptr);
	ptr->stop();
	ptr->update();
	Blocco * b = map.getBlocco(ptr->getPosition());
	if (b != NULL)
		b->cars.insert(ptr);

	graph.findPath(start, sources, ptr->percorso);
	ptr->nextBlock = ptr->percorso.get(0, false);
	ptr->currentBlock = ptr->nextBlock;
	spawned++;
}

void inputHandling(RenderWindow &widget)
{
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
		NUM_MACCHINE++;

#ifdef DEBUG_MODE
	if (Keyboard::isKeyPressed(Keyboard::Num1))
		MASK++;

	if (Keyboard::isKeyPressed(Keyboard::Num2))
		MASK--;
#endif // DEBUG_MODE


	static bool mousePressed = false;
	if (pause && !mousePressed && Mouse::isButtonPressed(Mouse::Left))
	{
		mousePressed = true;
		Vector2i mousePos = Mouse::getPosition(widget);
		std::cerr << "Mouse: " << mousePos.x << ", " << mousePos.y << "\n";
		Blocco * b = map.getBlocco(mousePos);

		if (b != NULL)
		{
			for (int i = 0; i < b->cars.count(); i++)
			{
				Macchina * ptr = b->cars.get(i);
				if (ptr->collider.contains(mousePos))
				{
					if (Keyboard::isKeyPressed(Keyboard::LAlt))
					{
						replaceCar(*ptr);
						ptr->update();
					}

					std::cerr << ptr->getPosition().x << ", " << ptr->getPosition().y << " " << toString(ptr->getDirection()) << " " << toString(ptr->currentDir) << " ";
					if (ptr->stoppedBy != NULL)
						std::cerr << ptr->stoppedBy->getPosition().x << ", " << ptr->stoppedBy->getPosition().y << " " << toString(ptr->stoppedBy->getDirection()) << " " << toString(ptr->stoppedBy->currentDir) << " ";

					std::cerr << "\n";
					break;
				}
			}
		}

	}
	else
		if (mousePressed && !Mouse::isButtonPressed(Mouse::Left))
			mousePressed = false;

}