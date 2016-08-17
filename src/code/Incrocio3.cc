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

	pos1.x = 19 + (size*columnIndex);
	pos1.y = 19 + (size*rowIndex);
	D1(PRINT("pos1=" << pos1.x << "," << pos1.y));

	pos2.x = 48 + (size*columnIndex);
	pos2.y = 19 + (size*rowIndex);
	D1(PRINT("pos2=" << pos2.x << "," << pos2.y));

	pos3.x = 19 + (size*columnIndex);
	pos3.y = 48 + (size*rowIndex);

	pos4.x = 48 + (size*columnIndex);
	pos4.y = 48 + (size*rowIndex);

	shape_1 = RectangleShape();
	shape_1.setPosition(pos1.x, pos1.y);
	shape_1.setSize(Vector2f(1, 1));
	shape_1.setFillColor(Color::Yellow);

	shape_2 = RectangleShape();
	shape_2.setPosition(pos2.x, pos2.y);
	shape_2.setSize(Vector2f(1, 1));
	shape_2.setFillColor(Color::Yellow);

	shape_3 = RectangleShape();
	shape_3.setPosition(pos3.x, pos3.y);
	shape_3.setSize(Vector2f(1, 1));
	shape_3.setFillColor(Color::Yellow);

	shape_4 = RectangleShape();
	shape_4.setPosition(pos4.x, pos4.y);
	shape_4.setSize(Vector2f(1, 1));
	shape_4.setFillColor(Color::Yellow);
}

void Incrocio3::cambiaVerso(TipoBlocco verso)
{
	if (verso == TipoBlocco::CROSS3_UP || verso == TipoBlocco::CROSS3_DOWN || verso == TipoBlocco::CROSS3_SX || verso == TipoBlocco::CROSS3_DX)
		setTipo(verso);
	else
		setTipo(TipoBlocco::CROSS3_UP);
}
