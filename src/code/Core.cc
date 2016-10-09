#include "../header/Core.h"

using namespace sf;

static Mappa map;

/** Grafo corrispondente alla mappa. */
static Graph graph;

/** Lista delle macchine create. */
static Macchina_List carList;

/** Lista delle sorgenti nella mappa. */
static Vector2i_List sources;

/** Lista dei puntatori alle macchine che sono in attesa di essere spawnate. */
static MacchinaPtr_List pendingCarList;

static int NUM_MACCHINE = 100,
			carSpawned = 0;

static float carSpawningTime = 0.25f,
				semChangeStatusTime = 10;

extern int RESX, RESY;
static Vector2i mapSize;

#ifdef DEBUG_MODE
extern unsigned int MASK;
#endif // DEBUG_MODE

static bool pause = false;
static bool inputEnabled = true;

/** Crea e aggiunge una nuova macchina su di una sorgente casuale aggiungendola all'apposita lista. 
*	Si genera il percorso che deve effettuare verso un'altra sorgente.
*/
void createCar();

/** Restituisce la posizione della sorgente sulla finestra e la direzione iniziale da intraprendere.
*
*	@param source Coordinate sorgente relative agli indici della matrice.
*	@param d Direzione iniziale.
*/
void placeCar(Vector2i &source, Direzione &d);

/** Riposiziona la macchina, creando un nuovo percorso e aggiungendola alla lista delle macchine da essere spawnate.
*
*	@param car Macchina da riposizionare.
*/
void replaceCar(Macchina &car);

/** Aggiorna la posizione della macchina:
*	- Se la macchina si trova su una curva o su un incrocio, cambia la direzione della macchina.\n
*	- Se si trova in prossimità di un semaforo si ferma.\n
*
*	Inoltre, si tiene traccia della porzione del percorso che manca da compiere (prossimo blocco e direzione) e
*	si aggiunge la macchina alla lista delle macchine dei blocchi con cui è in intersezione.
*
*	@param car Macchina da aggiornare.
*/
void updateCar(Macchina &car);

/** Trova la direzione da intraprendere per raggiungere il prossimo blocco partendo dalla posizione attuale.
*
*	@param carPos Posizione della macchina.
*	@param nextBlock Posizione prossimo blocco.
*	@return Prossima direzione.
*/
Direzione findDir(Vector2i carPos, Vector2i nextBlock);

/** Appena la macchina entra in un nuovo blocco, si aggiunge la macchina alle liste dei blocchi e si
*	aggiorna lo stato del percorso svolto e il prossimo blocco da raggiungere.
*
*	@param car Macchina da considerare.
*/
void carOnEnterBlock(Macchina &car);

/** Gestione degli input. Tasti:
*
*	R: Rigenera la mappa.
*	P: Pausa/Riprendi.
*	Mouse Sx: Riposiziona macchina.
*	A: Riposiziona macchina casuale.
*	1: Attiva/disattiva livello 1 di debug.
*	2: Attiva/disattiva livello 2 di debug.
*	3: Attiva/disattiva livello 3 di debug.
*/
void inputHandling(RenderWindow &widget);

/** Handler della pressione di un tasto della tastiera.
*
*	@param key Tasto.
*	@param keyPressed Stato di pressione del tasto.
*	@param handler Funziona da eseguire alla pressione del tasto.
*/
void keyHandler(Keyboard::Key key, bool &keyPressed, void (* handler) ());

/** Attiva/disattiva lo stato di pausa alla pressione del relativo tasto. */
void pauseKeyPressed();

/** Riposiziona la macchina al click del mouse. */
void mouseClicked(RenderWindow &widget);

#ifdef DEBUG_MODE
void changeMask(Keyboard::Key key, bool &keyPressed, int val)
{
	if (!keyPressed && Keyboard::isKeyPressed(key))
	{
		if (MASK & val)
			MASK -= val;
		else
			MASK += val;

		keyPressed = true;
		PRINT("Maschera impostata a: " << MASK);
}
	else
		if (keyPressed && !Keyboard::isKeyPressed(key))
			keyPressed = false;
}
#endif // DEBUG_MODE

void CoreInit()
{
	map.generate();
	mapSize = map.getMapSize();
	graph.buildGraph(map);
	
	map.getSorgenti(sources);
}

void enableInput(bool enable)
{
	inputEnabled = enable;
}

void update(RenderWindow &widget)
{
	static Time timeLastSpawned, timeLastSemUpdate;
	static Clock clocks;

	map.draw(widget);

	Time elapsedTime = clocks.restart();

	if (!pause)
	{
		timeLastSpawned += elapsedTime;
		timeLastSemUpdate += elapsedTime;
		map.checkCarCollision();
	}

	if (!pause && timeLastSpawned.asSeconds() >= carSpawningTime)
	{
		if (carSpawned < NUM_MACCHINE)
			createCar();

		timeLastSpawned = Time::Zero;
	}

	if (!pause && timeLastSemUpdate.asSeconds() >= semChangeStatusTime)
	{
		map.updateSemaphores();
		timeLastSemUpdate = Time::Zero;
	}

	if (inputEnabled)
		inputHandling(widget);
	
	for (int i = 0; i < carSpawned; i++)
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
}

Direzione findDir(Vector2i carPos, Vector2i nextBlock)
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
	Vector2i carPos = car.getPosition();

	//Si prende il blocco su cui si trova la macchina.
	Blocco * b = map.getBlocco(Vector2i(carPos.x, carPos.y)); 

	if (b != NULL)
	{
		//Se si è entrati in un nuovo blocco.
		if (carPos.x / Blocco::size == car.nextBlock.x && carPos.y / Blocco::size == car.nextBlock.y)
			carOnEnterBlock(car);

		Blocco * blocco = map.getBlocco(car.nextBlock.y, car.nextBlock.x);
		if (blocco != NULL)
			//Si aggiunge la macchina alla lista macchine del blocco successivo.
			blocco->cars.insert(&car);
		
		//Se il prossimo blocco è un incrocio, si controlla lo stato del semaforo e nel caso la macchina viene fermata.
		Incroci * i = dynamic_cast<Incroci *>(blocco);
		if (i != NULL)
		{
			IntRect sem = i->getSemaphore(car.currentDir);
			if (car.collider.intersects(sem))
				car.stop();
			else
				car.changeDirection(car.currentDir);
		}

		//In base al blocco in cui ci si trova e alla posizione della macchina, si cambia la sua direzione.
		Curva * c = dynamic_cast<Curva *>(b);
		if (c != NULL)
			car.changeDirection(c->getChangeDir(car.getShape().getPosition()));
		
		//In questo caso viene anche considerata la prossima direzione, stabilita quando si entra in
		//un nuovo blocco.
		i = dynamic_cast<Incroci *>(b);
		if (i != NULL)
			car.changeDirection(i->getChangeDir(car.getShape().getPosition(), car.nextDir));
		
		if (isEmptyBlock(b->getTipo()))
			replaceCar(car);
	}
	else
		replaceCar(car);
}

void carOnEnterBlock(Macchina & car)
{
	car.currentBlock = car.nextBlock;

	Blocco * blocco = map.getBlocco(car.currentBlock.y, car.currentBlock.x);
	if (car.collider.intersects(blocco->collider))
		//Aggiungo la macchina al blocco corrente.
		blocco->cars.insert(&car);

	//Trovo il prossimo nodo nel percorso se ho raggiunto il nodo attuale.
	if (car.nextBlock == car.percorso.get(0, false) && car.percorso.count() > 1)
	{
		car.percorso.get(0, true);
		car.nextDir = findDir(car.getPosition(), car.percorso.get(0, false));
	}

	//Trovo il prossimo blocco.
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
		x1 = RESX % Blocco::size;
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
		y1 = RESY % Blocco::size;
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

	for (int i = 0; i < carSpawned; i++)
		replaceCar(*carList.get(i));
}

void createCar()
{
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
	carSpawned++;
}

void inputHandling(RenderWindow &widget)
{
	static bool refreshPressed = false, 
				pausePressed = false;

	keyHandler(Keyboard::R, refreshPressed, &refreshMap);
	keyHandler(Keyboard::P, pausePressed, &pauseKeyPressed);
	

	static bool mousePressed = false;
	if (!mousePressed && Mouse::isButtonPressed(Mouse::Left))
	{
		mousePressed = true;
		mouseClicked(widget);
	}
	else
		if (mousePressed && !Mouse::isButtonPressed(Mouse::Left))
			mousePressed = false;

#ifdef DEBUG_MODE
	if (Keyboard::isKeyPressed(Keyboard::A))
		replaceCar(*carList.get(rand() % carList.count()));

	static bool num1Pressed = false, 
				num2Pressed = false, 
				num3Pressed = false;

	changeMask(Keyboard::Num1, num1Pressed, 1);
	changeMask(Keyboard::Num2, num2Pressed, 2);
	changeMask(Keyboard::Num3, num3Pressed, 4);
#endif // DEBUG_MODE

}

void keyHandler(Keyboard::Key key, bool & keyPressed, void(*handler)())
{
	if (!keyPressed && Keyboard::isKeyPressed(key))
	{
		keyPressed = true;
		handler();	
	}
	else
		if (keyPressed && !Keyboard::isKeyPressed(key))
			keyPressed = false;
}

void pauseKeyPressed()
{
	pause = !pause;
}

void mouseClicked(RenderWindow &widget)
{
	Vector2i mousePos = Mouse::getPosition(widget);

	D1(PRINT("Mouse: " << mousePos.x << ", " << mousePos.y));
	Blocco * b = map.getBlocco(mousePos);

	if (b != NULL)
		for (int i = 0; i < b->cars.count(); i++)
		{
			Macchina * ptr = b->cars.get(i);
			if (ptr->collider.contains(mousePos))
			{
				replaceCar(*ptr);
				ptr->update();

				D2(
					std::cout << ptr->getPosition().x << ", " << ptr->getPosition().y << " " << toString(ptr->getDirection()) << " " << toString(ptr->currentDir) << " ";
					if (ptr->stoppedBy != NULL)
						std::cout << ptr->stoppedBy->getPosition().x << ", " << ptr->stoppedBy->getPosition().y << " " << toString(ptr->stoppedBy->getDirection()) << " " << toString(ptr->stoppedBy->currentDir) << " ";

					std::cout << "\n"
				);
				break;
			}
		}
}


void setNumCar(int num)
{
	if (num >= 0)
		NUM_MACCHINE = num;
}

void setSpawnCarTime(int ms)
{
	if (ms > 0)
		carSpawningTime = ms / 1000.0f;
}

void setSemChangeStatusTime(int s)
{
	if (s > 0)
		semChangeStatusTime = s;
}
