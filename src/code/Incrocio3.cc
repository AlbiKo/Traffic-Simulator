#include "../header/Incrocio3.h"
#include "../header/Debug.h"

Incrocio3::Incrocio3()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::CROSS3_UP);
}

Incrocio3::Incrocio3(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	cambiaVerso(tipo);
}

void Incrocio3::cambiaVerso(TipoBlocco verso)
{
	if (verso == TipoBlocco::CROSS3_UP || verso == TipoBlocco::CROSS3_DOWN || verso == TipoBlocco::CROSS3_SX || verso == TipoBlocco::CROSS3_DX)
		setTipo(verso);
	else
		setTipo(TipoBlocco::CROSS3_UP);
}
