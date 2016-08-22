#include "../header/Curva.h"
#include "../header/Debug.h"

Curva::Curva()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::SX_TO_UP);
	pos1.x = 19;
	pos1.y = 19;

	pos2.x = 48;
	pos2.y = 19;

	pos3.x = 19;
	pos3.y = 48;

	pos4.x = 48;
	pos4.y = 48;
}

Curva::Curva(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	collider = IntRect(columnIndex * size, rowIndex * size, size, size);
	cambiaVerso(tipo);
	
	pos1.x = 19+(size*columnIndex);
	pos1.y = 19+(size*rowIndex);
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
	shape_1.setFillColor(Color::Green);

	shape_2 = RectangleShape();
	shape_2.setPosition(pos2.x, pos2.y);
	shape_2.setSize(Vector2f(1, 1));
	shape_2.setFillColor(Color::Green);

	shape_3 = RectangleShape();
	shape_3.setPosition(pos3.x, pos3.y);
	shape_3.setSize(Vector2f(1, 1));
	shape_3.setFillColor(Color::Green);

	shape_4 = RectangleShape();
	shape_4.setPosition(pos4.x, pos4.y);
	shape_4.setSize(Vector2f(1, 1));
	shape_4.setFillColor(Color::Green);
	//DEBUG

}

void Curva::cambiaVerso(TipoBlocco verso)
{
	if (isCurveBlock(verso))
		setTipo(verso);
	else
		setTipo(TipoBlocco::SX_TO_UP);
}

Direzione Curva::getChangeDir(Vector2f pos)
{
	Vector2i posizione = Vector2i(pos.x, pos.y);
	switch (tipo)
	{
		case TipoBlocco::SX_TO_UP:
			if (posizione.x == pos4.x && posizione.y == pos4.y)
				return Direzione::SU;
			if (posizione.x == pos1.x && posizione.y == pos1.y)
				return Direzione::SX;
			break;
		case TipoBlocco::SX_TO_DOWN:
			if (posizione.x == pos3.x && posizione.y == pos3.y)
				return Direzione::GIU;
			if (posizione.x == pos2.x && posizione.y == pos2.y)
				return Direzione::SX;
			break;
		case TipoBlocco::DX_TO_UP:
			if (posizione.x == pos2.x && posizione.y == pos2.y)
				return Direzione::SU;
			if (posizione.x == pos3.x && posizione.y == pos3.y)
				return Direzione::DX;
			break;
		case TipoBlocco::DX_TO_DOWN:
			if (posizione.x == pos1.x && posizione.y == pos1.y)
				return Direzione::GIU;
			if (posizione.x == pos4.x && posizione.y == pos4.y)
				return Direzione::DX;
			break;
		default:;
	}
	return Direzione::ND;
}
