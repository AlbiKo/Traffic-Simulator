#include "../header/Blocco.h"
#include "../header/Debug.h"

using namespace sf;

extern Texture * texture[11];

Blocco::Blocco()
{
	sprite.setPosition(0,0);
	setTipo(VUOTO);
}

Blocco::~Blocco()
{
	Vector2i coord = coordBlocco();
	D2(PRINT("Distructo blocco (" <<coord.x  <<"," <<coord.y <<")"));
}

Blocco::Blocco(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	D3(PRINT("Constructo Blocco"));
	sprite.setPosition(columnIndex * size, rowIndex * size);
	setTipo(tipo);
}

void Blocco::setTipo(TipoBlocco tipo)
{
	if (tipo == VUOTO)
		this->tipo = VUOTO;
	else
	{
		this->tipo = tipo;
		sprite.setTexture(*texture[tipo]);
	}
}

void Blocco::cambiaVerso(TipoBlocco verso)
{
	D1(PRINT("Blocco verso"));

}
