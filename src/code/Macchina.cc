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

	moving = dir;
	return true;
}

void Macchina::stop()
{
	moving = Direzione::ND;
}

void Macchina::update()
{
	int velX = 1, velY = 1;

	switch (moving)
	{
	case Direzione::SU:
		velX = 0;
		velY = -1;
		break;
	case Direzione::GIU:
		velX = 0;
		break;
	case Direzione::DX:
		velY = 0;
		break;
	case Direzione::SX:
		velX = -1;
		velY = 0;
		break;
	default:
		return;
	}

	shape.move(velX * 1, velY * 1);
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

	setColor(Color::Red);

	//Inizializzazione collider
	colliderSx = IntRect();
	colliderDx = IntRect();
	colliderUp = IntRect();
	colliderDown = IntRect();

	colliderSx.width = SIZE / 2;
	colliderSx.height = SIZE;

	colliderDx.width = SIZE / 2;
	colliderDx.height = SIZE;

	colliderUp.width = SIZE;
	colliderUp.height = SIZE / 2;

	colliderDown.width = SIZE;
	colliderDown.height = SIZE / 2;

	updateColliders();
}

void Macchina::updateColliders()
{
	colliderSx.left = shape.getPosition().x - SIZE + 1;
	colliderSx.height = shape.getPosition().y - SIZE / 2;

	colliderDx.left = shape.getPosition().x + SIZE / 2 + 1;
	colliderDx.height = shape.getPosition().y - SIZE / 2;

	colliderUp.left = shape.getPosition().x - SIZE / 2;
	colliderUp.height = shape.getPosition().y - SIZE + 1;

	colliderDown.left = shape.getPosition().x - SIZE / 2;
	colliderDown.height = shape.getPosition().y + SIZE / 2 + 1;
}
