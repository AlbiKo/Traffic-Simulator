#include "../header/Incrocio3.h"
#include "../header/Debug.h"

Incrocio3::Incrocio3()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::CROSS3_UP);
	wrongDir = Direzione::GIU;
	setSemaphorePos();
}

Incrocio3::Incrocio3(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	collider = IntRect(columnIndex * size, rowIndex * size, size, size);

	cambiaVerso(tipo);

	setChangeDirPos();
	setSemaphorePos();

	//DEBUG
	shape_1.setPosition(pos1.x, pos1.y);
	shape_1.setSize(Vector2f(1, 1));
	shape_1.setFillColor(Color::Yellow);

	shape_2.setPosition(pos2.x, pos2.y);
	shape_2.setSize(Vector2f(1, 1));
	shape_2.setFillColor(Color::Yellow);

	shape_3.setPosition(pos3.x, pos3.y);
	shape_3.setSize(Vector2f(1, 1));
	shape_3.setFillColor(Color::Yellow);

	shape_4.setPosition(pos4.x, pos4.y);
	shape_4.setSize(Vector2f(1, 1));
	shape_4.setFillColor(Color::Yellow);
	//DEBUG
}

void Incrocio3::setWrongDir()
{
	switch (tipo)
	{
	case TipoBlocco::CROSS3_SX:
		wrongDir = Direzione::DX;
		break;
	case TipoBlocco::CROSS3_DX:
		wrongDir = Direzione::SX;
		break;
	case TipoBlocco::CROSS3_UP:
		wrongDir = Direzione::GIU;
		break;
	case TipoBlocco::CROSS3_DOWN:
		wrongDir = Direzione::SU;
		break;
	default:
		wrongDir = Direzione::ND;
		break;
	}
}

void Incrocio3::cambiaVerso(TipoBlocco verso)
{
	if (isCross3Block(verso))
		setTipo(verso);
	else
		setTipo(TipoBlocco::CROSS3_UP);

	setWrongDir();
}
