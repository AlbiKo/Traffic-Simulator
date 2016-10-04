#include "../header/Rettilineo.h"
#include "../header/Debug.h"

Rettilineo::Rettilineo()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::HORIZONTAL);

	collider = IntRect(0, 0, size, size);
}

Rettilineo::Rettilineo(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	cambiaVerso(tipo);

	collider = IntRect(columnIndex * size, rowIndex * size, size, size);
}

void Rettilineo::cambiaVerso(TipoBlocco verso)
{
	if (isRectBlock(verso))
		setTipo(verso);
	else
		setTipo(TipoBlocco::HORIZONTAL);
}

void Rettilineo::draw(RenderWindow & widget)
{
	widget.draw(sprite);
}
