#include "../header/Incrocio4.h"
#include "../header/Debug.h"

Incrocio4::Incrocio4()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::CROSS4);
}

Incrocio4::Incrocio4(int rowIndex, int columnIndex)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	setTipo(TipoBlocco::CROSS4);
}
