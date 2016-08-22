#include "../header/Rettilineo.h"
#include "../header/Debug.h"

Rettilineo::Rettilineo()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::HORIZONTAL);
}

Rettilineo::Rettilineo(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	//D1(PRINT("Constructo Rect"));
	sprite.setPosition(columnIndex * size, rowIndex * size);
	collider = IntRect(columnIndex * size, rowIndex * size, size, size);
	cambiaVerso(tipo);
}

void Rettilineo::cambiaVerso(TipoBlocco verso)
{
	//D1(PRINT("rett verso"));
	if (verso == TipoBlocco::HORIZONTAL || verso == TipoBlocco::VERTICAL)
		setTipo(verso);
	else
		setTipo(TipoBlocco::HORIZONTAL);


}
