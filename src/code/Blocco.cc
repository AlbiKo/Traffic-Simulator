#include "../header/Blocco.h"
#include "../header/Debug.h"

using namespace sf;

extern Texture * texture[11];

Blocco::Blocco()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::EMPTY);
	
	//collider = IntRect(0, 0, Blocco::size, Blocco::size);
}

Blocco::~Blocco()
{
	//Vector2i coord = coordBlocco();
	//D2(PRINT("Distructo blocco (" <<coordBlocco().x  <<"," <<coordBlocco().y <<")"));
}

Blocco::Blocco(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	//D3(PRINT("Constructo Blocco"));
	sprite.setPosition(columnIndex * size, rowIndex * size);
	collider = IntRect(columnIndex * size, rowIndex * size, size, size);
	setTipo(tipo);
}

bool Blocco::checkBlockCollision(Macchina & currentCar)
{
	if (currentCar.collider.intersects(collider))
		return true;

	if ((currentCar.getDirection() == Direzione::SX && currentCar.colliderSx.intersects(collider)) ||
		(currentCar.getDirection() == Direzione::DX && currentCar.colliderDx.intersects(collider)) ||
		(currentCar.getDirection() == Direzione::SU && currentCar.colliderUp.intersects(collider)) ||
		(currentCar.getDirection() == Direzione::GIU && currentCar.colliderDown.intersects(collider)))
			return true;

	return false;
}

bool Blocco::checkSameDirCollision(Macchina & currentCar, Macchina & collidingCar)
{
	if ((currentCar.currentDir == Direzione::DX && currentCar.collider.left < collidingCar.collider.left && currentCar.collider.top == collidingCar.collider.top) ||
		(currentCar.currentDir == Direzione::SX && currentCar.collider.left > collidingCar.collider.left && currentCar.collider.top == collidingCar.collider.top) ||
		(currentCar.currentDir == Direzione::SU && currentCar.collider.top > collidingCar.collider.top && currentCar.collider.left == collidingCar.collider.left) ||
		(currentCar.currentDir == Direzione::GIU && currentCar.collider.top < collidingCar.collider.top && currentCar.collider.left == collidingCar.collider.left) )
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
	//D1(PRINT("Blocco verso"));
}

void Blocco::checkCollision()
{
	for (int i = 0; i < cars.count(); i++)
	{
		Macchina * car = cars.get(i);
		
		if (car == NULL)
			continue;

		if (car != NULL && !checkBlockCollision(*car) )
			cars.remove(car);
		else
			for (int j = 0; j < cars.count(); j++)
			{
				Macchina * m = cars.get(j);

				if (m == NULL || m == car || car->getPosition() == m->getPosition())
					continue;

				if (car->isMoving() && car->collider.intersects(m->collider))
				{
					if (checkSameDirCollision(*car, *m) ||	checkCurveCollison(*car, *m) ||	checkCrossCollision(*car, *m) )
					{
						D3(std::cerr << toString(car->currentDir) << "   " << car->getPosition().x << ", " << car->getPosition().y <<"  ptr " <<car <<"  ptrBy  " <<m  <<"  " << m->getPosition().x << ", " << m->getPosition().y <<"  " << (isCrossBlock(tipo) && car->currentDir == Direzione::DX && car->colliderDx.intersects(m->collider)) <<"  " << (isCrossBlock(tipo) && car->currentDir == Direzione::SX && car->colliderSx.intersects(m->collider)) <<"  " << (isCrossBlock(tipo) && car->currentDir == Direzione::SU && car->colliderUp.intersects(m->collider)) <<"   " << (isCrossBlock(tipo) && car->currentDir == Direzione::GIU && car->colliderDown.intersects(m->collider)) <<"\n");
						car->carBy = m;
						car->stop();
						break;
					}
				}
				else
					if (car->carBy == m  && !car->collider.intersects(m->collider))
						car->changeDirection(car->currentDir);
			}
	}
}