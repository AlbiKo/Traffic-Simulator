#include "../header/Blocco.h"
#include "../header/Debug.h"

using namespace sf;

extern Texture * texture[11];

Blocco::Blocco()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::EMPTY);	
}

Blocco::~Blocco()
{
}

Blocco::Blocco(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	collider = IntRect(columnIndex * size, rowIndex * size, size, size);
	setTipo(tipo);
}

bool Blocco::checkBlockCollision(Macchina & currentCar)
{
	if (currentCar.collider.intersects(collider))
		return true;

	if ((currentCar.colliderSx.intersects(collider)) ||	(currentCar.colliderDx.intersects(collider)) ||
		(currentCar.colliderUp.intersects(collider)) ||	(currentCar.colliderDown.intersects(collider)))
			return true;

	return false;
}

bool Blocco::checkSameDirCollision(Macchina & currentCar, Macchina & collidingCar)
{
	if ((currentCar.currentDir == Direzione::DX && currentCar.currentDir == Direzione::DX && currentCar.collider.left < collidingCar.collider.left && currentCar.collider.top == collidingCar.collider.top) ||
		(currentCar.currentDir == Direzione::SX && currentCar.currentDir == Direzione::SX && currentCar.collider.left > collidingCar.collider.left && currentCar.collider.top == collidingCar.collider.top) ||
		(currentCar.currentDir == Direzione::SU && currentCar.currentDir == Direzione::SU && currentCar.collider.top > collidingCar.collider.top && currentCar.collider.left == collidingCar.collider.left) ||
		(currentCar.currentDir == Direzione::GIU && currentCar.currentDir == Direzione::GIU && currentCar.collider.top < collidingCar.collider.top && currentCar.collider.left == collidingCar.collider.left) )
		return true;

	return false;
}

bool Blocco::checkCurveCollison(Macchina & currentCar, Macchina & collidingCar)
{
	if (!isCurveBlock(tipo))
		return false;

	if ((currentCar.currentDir == Direzione::SU && (collidingCar.currentDir == Direzione::SX || collidingCar.currentDir == Direzione::DX) && currentCar.collider.top > collidingCar.collider.top) ||
		(currentCar.currentDir == Direzione::GIU && (collidingCar.currentDir == Direzione::SX || collidingCar.currentDir == Direzione::DX) && currentCar.collider.top < collidingCar.collider.top) ||
		(currentCar.currentDir == Direzione::DX && (collidingCar.currentDir == Direzione::SU || collidingCar.currentDir == Direzione::GIU) && currentCar.collider.left < collidingCar.collider.left) ||
		(currentCar.currentDir == Direzione::SX && (collidingCar.currentDir == Direzione::SU || collidingCar.currentDir == Direzione::GIU) && currentCar.collider.left > collidingCar.collider.left))
		return true;

	return false;
}

bool Blocco::checkCrossCollision(Macchina & currentCar, Macchina & collidingCar)
{
	if (!isCrossBlock(tipo))
		return false;

	if ((currentCar.currentDir == Direzione::DX && currentCar.colliderDx.intersects(collidingCar.collider) && std::abs(currentCar.getPosition().y - collidingCar.getPosition().y) < std::abs(currentCar.getPosition().x - collidingCar.getPosition().x)) ||
		(currentCar.currentDir == Direzione::SX && currentCar.colliderSx.intersects(collidingCar.collider) && std::abs(currentCar.getPosition().y - collidingCar.getPosition().y) < std::abs(currentCar.getPosition().x - collidingCar.getPosition().x)) ||
		(currentCar.currentDir == Direzione::SU && currentCar.colliderUp.intersects(collidingCar.collider) && std::abs(currentCar.getPosition().x - collidingCar.getPosition().x) < 20) ||
		(currentCar.currentDir == Direzione::GIU && currentCar.colliderDown.intersects(collidingCar.collider) && std::abs(currentCar.getPosition().x - collidingCar.getPosition().x) < 20))
		return true;

	return false;
}

void Blocco::setTipo(TipoBlocco tipo)
{
	if (tipo == TipoBlocco::EMPTY)
		this->tipo = TipoBlocco::EMPTY;
	else
	{
		this->tipo = tipo;
		sprite.setTexture(*texture[toInt(tipo)]);
	}
}

void Blocco::cambiaVerso(TipoBlocco verso)
{
}

void Blocco::draw(RenderWindow & widget)
{
}

void Blocco::checkCollision()
{
	for (int i = 0; i < cars.count(); i++)
	{
		Macchina * car = cars.get(i);
		
		if (car == NULL || !car->drawable)
			continue;

		if (car != NULL && !checkBlockCollision(*car))
		{
			cars.remove(car);
			i--;
		}
		else
			for (int j = 0; j < cars.count(); j++)
			{
				Macchina * m = cars.get(j);

				if (m == NULL || m == car || car->getPosition() == m->getPosition() || !m->drawable)
					continue;

				if (car->isMoving() && car->collider.intersects(m->collider))
				{
					if (checkSameDirCollision(*car, *m) || checkCurveCollison(*car, *m) || checkCrossCollision(*car, *m))
					{
						car->stoppedBy = m;
						car->stop();
						break;
					}
				}
				else
					if ((car->stoppedBy == m && !car->collider.intersects(m->collider)) || (car->stoppedBy != NULL && !car->collider.intersects(car->stoppedBy->collider)))
					{
						car->changeDirection(car->currentDir);
						car->stoppedBy = NULL;
					}
			}
	}
}

bool Blocco::canBeSpawned(Macchina & car)
{
	bool occupied = true, draw = false;
	assert(!car.drawable);
	int deltaX = -1, deltaY = -1;

	for (int i = 0; i < cars.count(); i++)
	{
		Macchina * ptr = cars.get(i);
		
		deltaX = std::abs(car.getPosition().x - ptr->getPosition().x);
		deltaY = std::abs(car.getPosition().y - ptr->getPosition().y);

		if ((ptr != &car) &&
			(((car.currentDir == Direzione::SX || car.currentDir == Direzione::DX) && deltaX < 20 && car.getPosition().y == ptr->getPosition().y) ||
			((car.currentDir == Direzione::GIU || car.currentDir == Direzione::SU) && deltaY < 20 && car.getPosition().x == ptr->getPosition().x)))
		{
			occupied = false;
			
			if (ptr->drawable)
			{
				draw = true;
				break;
			}
		}
	}

	if (!occupied && draw)
		return false;
	
	return true;
}
