#include "../header/Curva.h"
#include "../header/Debug.h"

Curva::Curva()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::SX_TO_UP);
	int secondaryOffset = size - offsetChangeDirPos - 1;
	pos1.x = offsetChangeDirPos;
	pos1.y = offsetChangeDirPos;

	pos2.x = secondaryOffset;
	pos2.y = offsetChangeDirPos;

	pos3.x = offsetChangeDirPos;
	pos3.y = secondaryOffset;

	pos4.x = secondaryOffset;
	pos4.y = secondaryOffset;
}

Curva::Curva(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	collider = IntRect(columnIndex * size, rowIndex * size, size, size);
	cambiaVerso(tipo);
	
	int secondaryOffset = size - offsetChangeDirPos - 1;

	pos1.x = offsetChangeDirPos+(size*columnIndex);
	pos1.y = offsetChangeDirPos+(size*rowIndex);
	D1(PRINT("pos1=" << pos1.x << "," << pos1.y));

	pos2.x = secondaryOffset + (size*columnIndex);
	pos2.y = offsetChangeDirPos + (size*rowIndex);
	D1(PRINT("pos2=" << pos2.x << "," << pos2.y));

	pos3.x = offsetChangeDirPos + (size*columnIndex);
	pos3.y = secondaryOffset + (size*rowIndex);

	pos4.x = secondaryOffset + (size*columnIndex);
	pos4.y = secondaryOffset + (size*rowIndex);

#ifdef DEBUG_MODE
	shape_1.setPosition(pos1.x, pos1.y);
	shape_1.setSize(Vector2f(1, 1));
	shape_1.setFillColor(Color::Green);

	shape_2.setPosition(pos2.x, pos2.y);
	shape_2.setSize(Vector2f(1, 1));
	shape_2.setFillColor(Color::Green);

	shape_3.setPosition(pos3.x, pos3.y);
	shape_3.setSize(Vector2f(1, 1));
	shape_3.setFillColor(Color::Green);

	shape_4.setPosition(pos4.x, pos4.y);
	shape_4.setSize(Vector2f(1, 1));
	shape_4.setFillColor(Color::Green);
#endif // DEBUG_MODE
}

void Curva::cambiaVerso(TipoBlocco verso)
{
	if (isCurveBlock(verso))
		setTipo(verso);
	else
		setTipo(TipoBlocco::SX_TO_UP);
}

void Curva::draw(RenderWindow & widget)
{
	widget.draw(sprite);

	D1(
		widget.draw(shape_1);
		widget.draw(shape_2);
		widget.draw(shape_3);
		widget.draw(shape_4);
	);
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
