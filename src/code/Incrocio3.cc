#include "../header/Incrocio3.h"
#include "../header/Debug.h"

Incrocio3::Incrocio3()
{
	sprite.setPosition(0,0);
	setTipo(CROSS3_UP);
}

Incrocio3::Incrocio3(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	cambiaVerso(tipo);
}

void Incrocio3::cambiaVerso(TipoBlocco verso)
{
	if (verso == CROSS3_UP || verso == CROSS3_DOWN || verso == CROSS3_SX || CROSS3_DX)
		setTipo(verso);
	else
		setTipo(CROSS3_UP);
}
