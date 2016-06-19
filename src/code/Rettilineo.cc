#include "../header/Rettilineo.h"
#include "../header/Debug.h"

Rettilineo::Rettilineo()
{
	sprite.setPosition(0,0);
	setTipo(ORIZZONTALE);
}

Rettilineo::Rettilineo(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	D1(PRINT("Constructo Rect"));
	sprite.setPosition(columnIndex * size, rowIndex * size);
	cambiaVerso(tipo);
}

void Rettilineo::cambiaVerso(TipoBlocco verso)
{
	D1(PRINT("rett verso"));
	if (verso == ORIZZONTALE || verso == VERTICALE)
		setTipo(verso);
	else
		setTipo(ORIZZONTALE);


}
