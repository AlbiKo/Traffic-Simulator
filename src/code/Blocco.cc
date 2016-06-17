#include "../header/Blocco.h"
#include "../header/Debug.h"

using namespace sf;

Blocco::Blocco(int rowIndex, int columnIndex, TipoBlocco tipo, Texture * texture)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	setTipo(tipo, texture);
}

void Blocco::setTipo(TipoBlocco tipo,  Texture * texture)
{
	if (tipo == VUOTO || texture == NULL)
		this->tipo = VUOTO;
	else
	{
		this->tipo = tipo;
		sprite.setTexture(*texture);
	}
}
