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

	//DEBUG
	shape_1 = RectangleShape();
	shape_1.setPosition(pos1.x, pos1.y);
	shape_1.setSize(Vector2f(1, 1));
	shape_1.setFillColor(Color::Red);

	shape_2 = RectangleShape();
	shape_2.setPosition(pos2.x, pos2.y);
	shape_2.setSize(Vector2f(1, 1));
	shape_2.setFillColor(Color::Red);

	shape_3 = RectangleShape();
	shape_3.setPosition(pos3.x, pos3.y);
	shape_3.setSize(Vector2f(1, 1));
	shape_3.setFillColor(Color::Red);

	shape_4 = RectangleShape();
	shape_4.setPosition(pos4.x, pos4.y);
	shape_4.setSize(Vector2f(1, 1));
	shape_4.setFillColor(Color::Red);
	//DEBUG
}

Direzione Incrocio4::getChangeDir(Vector2f pos, Direzione dir)
{
	//Direzione dir = Direzione::DX;
	Vector2i posizione = Vector2i(pos.x, pos.y);
	switch (dir)
	{
		case Direzione::SU:
			if (posizione.x == pos1.x && posizione.y == pos1.y)
				return Direzione::DX;
			if ((posizione.x == pos2.x && posizione.y == pos2.y) || (posizione.x == pos4.x && posizione.y == pos4.y))
				return Direzione::SU;
			break;
		case Direzione::GIU:
			if (posizione.x == pos4.x && posizione.y == pos4.y)
				return Direzione::SX;
			if ((posizione.x == pos1.x && posizione.y == pos1.y) || (posizione.x == pos3.x && posizione.y == pos3.y))
				return Direzione::GIU;
			break;
		case Direzione::DX:
			if (posizione.x == pos2.x && posizione.y == pos2.y)
				return Direzione::GIU;
			if ((posizione.x == pos4.x && posizione.y == pos4.y) || (posizione.x == pos3.x && posizione.y == pos3.y))
				return Direzione::DX;
			break;
		case Direzione::SX:
			if (posizione.x == pos3.x && posizione.y == pos3.y)
				return Direzione::SU;
			if ((posizione.x == pos1.x && posizione.y == pos1.y) || (posizione.x == pos2.x && posizione.y == pos2.y))
				return Direzione::SX;
			break;
		default:
			break;
	}
	return Direzione::ND;
}


