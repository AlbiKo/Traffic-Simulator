#include "../header/Macchina.h"
#include "../header/Debug.h"

Macchina::Macchina()
{
	
	D1(PRINT("Costruzione macchina"));
	initMacchina();
}

Macchina::Macchina(Vector2i pos)
{
	initMacchina();
	setPosition(pos);

	updateColliders();
}

void Macchina::draw(RenderWindow & widget)
{
//	widget.draw(globo);
	
	widget.draw(shape);

/*	widget.draw(sx);
	widget.draw(dx);
	widget.draw(up);
	widget.draw(down);
	*/
	
}

Macchina::~Macchina()
{
	D1(PRINT("Distruzione macchina"));
}

void Macchina::setPosition(Vector2i pos)
{
	shape.setPosition(pos.x, pos.y);
}

void Macchina::setColor(Color c)
{
	shape.setFillColor(c);
}

RectangleShape Macchina::getShape()
{
	return shape;
}

Color Macchina::getColor()
{
	return shape.getFillColor();
}

void Macchina::copy(Macchina m)
{
	setPosition(m.getPosition());
	setColor(m.getColor());
	moving = m.moving;
	currentDir = m.currentDir;
}

bool Macchina::isMoving()
{
	if (moving == Direzione::ND)
		return false;

	return true;
}

bool Macchina::changeDirection(Direzione dir)
{
	if (dir == Direzione::ND || dir == moving)
		return false;

	currentDir = dir;
	moving = dir;
	return true;
}

void Macchina::stop()
{
	moving = Direzione::ND;
}

void Macchina::update()
{
	int baseX = 1, baseY = 1;

	switch (moving)
	{
	case Direzione::SU:
		baseX = 0;
		baseY = -1;
		break;
	case Direzione::GIU:
		baseX = 0;
		break;
	case Direzione::DX:
		baseY = 0;
		break;
	case Direzione::SX:
		baseX = -1;
		baseY = 0;
		break;
	default:
		baseX = 0;
		baseY = 0;
		break;
	}

	shape.move(baseX * SPEED, baseY * SPEED);
	updateColliders();
}

Direzione Macchina::getDirection()
{
	return moving;
}

void Macchina::initMacchina()
{
	moving = Direzione::ND; 

	//Inizializzazione sprite macchina
	shape = RectangleShape();

	shape.setPosition(-SIZE / 2, -SIZE / 2);
	shape.setOrigin(SIZE / 2, SIZE / 2);
	shape.setSize(Vector2f(SIZE, SIZE));
	shape.setOutlineThickness(-1);

	setColor(Color(rand() % 210 + 30, rand() % 210 + 30, rand() % 200 + 30, 255));

	//Inizializzazione collider
	colliderSx = IntRect();
	colliderDx = IntRect();
	colliderUp = IntRect();
	colliderDown = IntRect();

	collider = IntRect(getPosition().x - SIZE / 2 - 3, getPosition().y - SIZE / 2 - 3, SIZE + 4, SIZE + 4);
	globo = RectangleShape(Vector2f(SIZE + 4, SIZE + 4));
	globo.setFillColor(Color::Red);

	sx = RectangleShape(Vector2f(10, SIZE));
	sx.setFillColor(Color::Green);

	dx = RectangleShape(Vector2f(10, SIZE));
	dx.setFillColor(Color::Green);

	up = RectangleShape(Vector2f(SIZE, 10));
	up.setFillColor(Color::Green);

	down = RectangleShape(Vector2f(SIZE, 10));
	down.setFillColor(Color::Green);


	colliderSx.width = 10;
	colliderSx.height = SIZE;

	colliderDx.width = 10;
	colliderDx.height = SIZE;

	colliderUp.width = SIZE;
	colliderUp.height = 10;

	colliderDown.width = SIZE;
	colliderDown.height = 10;

	updateColliders();
}

void Macchina::updateColliders()
{

	globo.setPosition(getPosition().x - SIZE / 2 - 2, getPosition().y - SIZE / 2 - 2);

	sx.setPosition(getPosition().x - SIZE / 2 - 10, getPosition().y - SIZE/2);
	dx.setPosition(getPosition().x + SIZE / 2 + 1, getPosition().y - SIZE/2);
	up.setPosition(getPosition().x - SIZE/2, getPosition().y - SIZE / 2 - 10);
	down.setPosition(getPosition().x - SIZE/2, getPosition().y + SIZE / 2 + 1);

	colliderSx.left = getPosition().x - SIZE/2 - 10;
	colliderSx.top = getPosition().y - SIZE/2;

	colliderDx.left = getPosition().x + SIZE / 2 + 1;
	colliderDx.top = getPosition().y - SIZE/2;

	colliderUp.left = getPosition().x - SIZE/2;
	colliderUp.top = getPosition().y - SIZE/2 - 10;

	colliderDown.left = getPosition().x - SIZE/2;
	colliderDown.top = getPosition().y + SIZE / 2 + 1;

	
	collider.left = getPosition().x - SIZE / 2 - 2;
	collider.top = getPosition().y - SIZE / 2 - 2;
}
