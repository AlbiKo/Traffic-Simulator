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

bool pause = false;


void createCar();
void placeCar(Vector2i &source, Direzione &d);
void replaceCar(Macchina &car);
void replaceCarOnClick(Macchina &car);
/**Aggiorna la posizione della macchina\n
*  Se la macchina si trova su una curva o su un incrocio, cambia la direzione della macchina*/
void updateCar(Macchina &car);
void refreshMap();

void inputHandling(RenderWindow &widget);

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

void replaceCarOnClick(Macchina &car)
{
	replaceCar(car);
	car.update();

	D2(
		std::cout << car.getPosition().x << ", " << car.getPosition().y << " " << toString(car.getDirection()) << " " << toString(car.currentDir) << " ";
		if (car.stoppedBy != NULL)
			std::cout << car.stoppedBy->getPosition().x << ", " << car.stoppedBy->getPosition().y << " " << toString(car.stoppedBy->getDirection()) << " " << toString(car.stoppedBy->currentDir) << " ";

		std::cout << "\n"
	);
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
	static bool refreshPressed = false;
	if (!refreshPressed && Keyboard::isKeyPressed(Keyboard::R))
	{
		refreshMap();
		refreshPressed = true;
	}
	else
		if (refreshPressed && !Keyboard::isKeyPressed(Keyboard::R))
			refreshPressed = false;

	static bool pausePressed = false;
	if (!pausePressed && Keyboard::isKeyPressed(Keyboard::P))
	{
		pause = !pause;
		pausePressed = true;
	}
	else
		if (pausePressed && !Keyboard::isKeyPressed(Keyboard::P))
			pausePressed = false;

#ifdef DEBUG_MODE
	if (Keyboard::isKeyPressed(Keyboard::A))
		replaceCar(*carList.get(rand() % carList.count()));

	static bool num1Pressed = false;
	if (!num1Pressed && Keyboard::isKeyPressed(Keyboard::Num1))
	{
		if (MASK & 1)
			MASK--;
		else
			MASK++;

		num1Pressed = true;
		PRINT("Maschera impostata a: " <<MASK);
}
	else
		if (num1Pressed && !Keyboard::isKeyPressed(Keyboard::Num1))
			num1Pressed = false;

	static bool num2Pressed = false;
	if (!num2Pressed && Keyboard::isKeyPressed(Keyboard::Num2))
	{
		if (MASK & 2)
			MASK -= 2;
		else
			MASK += 2;

		num2Pressed = true;
		PRINT("Maschero impostata a: " << MASK);
	}
	else
		if (num2Pressed && !Keyboard::isKeyPressed(Keyboard::Num2))
			num2Pressed = false;

	static bool num3Pressed = false;
	if (!num3Pressed && Keyboard::isKeyPressed(Keyboard::Num3))
	{
		if (MASK & 4)
			MASK -= 4;
		else
			MASK += 4;

		num3Pressed = true;
		PRINT("Maschera impostata a: " << MASK);
	}
	else
		if (num3Pressed && !Keyboard::isKeyPressed(Keyboard::Num3))
			num3Pressed = false;

#endif // DEBUG_MODE


	static bool mousePressed = false;
	if (!mousePressed && Mouse::isButtonPressed(Mouse::Left))
	{
		mousePressed = true;
		Vector2i mousePos = Mouse::getPosition(widget);

		D1(PRINT("Mouse: " << mousePos.x << ", " << mousePos.y));
		Blocco * b = map.getBlocco(mousePos);

		if (b != NULL)
			for (int i = 0; i < b->cars.count(); i++)
			{
				Macchina * ptr = b->cars.get(i);
				if (ptr->collider.contains(mousePos))
				{
					replaceCarOnClick(*ptr);
					break;
				}
			}
	}
	else
		if (mousePressed && !Mouse::isButtonPressed(Mouse::Left))
			mousePressed = false;

}