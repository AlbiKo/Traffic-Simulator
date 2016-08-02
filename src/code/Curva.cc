#include "../header/Curva.h"
#include "../header/Debug.h"

Curva::Curva()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::SX_TO_UP);
}

Curva::Curva(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	cambiaVerso(tipo);
}

void Curva::cambiaVerso(TipoBlocco verso)
{
	if (verso == TipoBlocco::SX_TO_UP || verso == TipoBlocco::SX_TO_DOWN || verso == TipoBlocco::DX_TO_UP || verso == TipoBlocco::DX_TO_DOWN)
		setTipo(verso);
	else
		setTipo(TipoBlocco::SX_TO_UP);
}
