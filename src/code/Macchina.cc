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
	widget.draw(shape);
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
		return;
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

	shape.setPosition(SIZE / 2, SIZE / 2);
	shape.setOrigin(SIZE / 2, SIZE / 2);
	shape.setSize(Vector2f(SIZE, SIZE));

	setColor(Color(rand() % 210 + 30, rand() % 210 + 30, rand() % 200 + 30, 255));

	//Inizializzazione collider
	colliderSx = IntRect();
	colliderDx = IntRect();
	colliderUp = IntRect();
	colliderDown = IntRect();

	collider = IntRect(getPosition().x - SIZE / 2 - 4, getPosition().y - SIZE / 2 - 4, SIZE + 4, SIZE + 4);

	colliderSx.width = SIZE;
	colliderSx.height = SIZE;

	colliderDx.width = SIZE;
	colliderDx.height = SIZE;

	colliderUp.width = SIZE;
	colliderUp.height = SIZE;

	colliderDown.width = SIZE;
	colliderDown.height = SIZE;

	updateColliders();
}

void Macchina::updateColliders()
{
	colliderSx.left = shape.getPosition().x - SIZE - SIZE/2 + 1;
	colliderSx.height = shape.getPosition().y - SIZE / 2;

	colliderDx.left = shape.getPosition().x + SIZE / 2 + 1;
	colliderDx.height = shape.getPosition().y - SIZE / 2;

	colliderUp.left = shape.getPosition().x - SIZE / 2;
	colliderUp.height = shape.getPosition().y - SIZE - SIZE/2 + 1;

	colliderDown.left = shape.getPosition().x - SIZE / 2;
	colliderDown.height = shape.getPosition().y + SIZE / 2 + 1;

	collider.left = getPosition().x - SIZE / 2 - 4;
	collider.top = getPosition().y - SIZE / 2 - 4;
}
