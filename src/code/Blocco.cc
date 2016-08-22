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
		assert(car != NULL);
		if (car != NULL && !car->collider.intersects(collider) &&
			((car->getDirection() == Direzione::SX && !car->colliderSx.intersects(collider))||
			(car->getDirection() == Direzione::DX && !car->colliderDx.intersects(collider))	|| 
			(car->getDirection() == Direzione::SU && !car->colliderUp.intersects(collider)) || 
			(car->getDirection() == Direzione::GIU && !car->colliderDown.intersects(collider)) ))
				cars.remove(car);
		else
			for (int j = 0; j < cars.count(); j++)
			{
					Macchina * m = cars.get(j);

					if (m == NULL || m == car)
						continue;

					if (car->isMoving() && car->collider.intersects(m->collider))
					{

						if ((car->currentDir == Direzione::DX && car->collider.left < m->collider.left) ||
							(car->currentDir == Direzione::SX && car->collider.left > m->collider.left) ||
							(car->currentDir == Direzione::SU && car->collider.top > m->collider.top) ||
							(car->currentDir == Direzione::GIU && car->collider.top < m->collider.top))
						{
							car->carBy = m;
							car->stop();
							break;
						}
						else
							if (car->carBy == m && !car->collider.intersects(m->collider))
								car->changeDirection(car->currentDir);
					}
					else
						if (car->carBy == m && !car->collider.intersects(m->collider))
							car->changeDirection(car->currentDir);

			}
	}
}