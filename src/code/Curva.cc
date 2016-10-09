#include "../header/Curva.h"
#include "../header/Debug.h"

Curva::Curva()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::SX_TO_UP);
	int secondaryOffset = size - offsetChangeDirPos - 1;
	posChangeDirTopLeft.x = offsetChangeDirPos;
	posChangeDirTopLeft.y = offsetChangeDirPos;

	posChangeDirTopRight.x = secondaryOffset;
	posChangeDirTopRight.y = offsetChangeDirPos;

	posChangeDirBottomLeft.x = offsetChangeDirPos;
	posChangeDirBottomLeft.y = secondaryOffset;

	posChangeDirBottomRight.x = secondaryOffset;
	posChangeDirBottomRight.y = secondaryOffset;
}

Curva::Curva(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	collider = IntRect(columnIndex * size, rowIndex * size, size, size);
	cambiaVerso(tipo);
	
	int secondaryOffset = size - offsetChangeDirPos - 1;

	posChangeDirTopLeft.x = offsetChangeDirPos+(size*columnIndex);
	posChangeDirTopLeft.y = offsetChangeDirPos+(size*rowIndex);
	D1(PRINT("posChangeDirTopLeft=" << posChangeDirTopLeft.x << "," << posChangeDirTopLeft.y));

	posChangeDirTopRight.x = secondaryOffset + (size*columnIndex);
	posChangeDirTopRight.y = offsetChangeDirPos + (size*rowIndex);
	D1(PRINT("posChangeDirTopRight=" << posChangeDirTopRight.x << "," << posChangeDirTopRight.y));

	posChangeDirBottomLeft.x = offsetChangeDirPos + (size*columnIndex);
	posChangeDirBottomLeft.y = secondaryOffset + (size*rowIndex);

	posChangeDirBottomRight.x = secondaryOffset + (size*columnIndex);
	posChangeDirBottomRight.y = secondaryOffset + (size*rowIndex);

#ifdef DEBUG_MODE
	shapeTopLeft.setPosition(posChangeDirTopLeft.x, posChangeDirTopLeft.y);
	shapeTopLeft.setSize(Vector2f(1, 1));
	shapeTopLeft.setFillColor(Color::Green);

	shapeTopRight.setPosition(posChangeDirTopRight.x, posChangeDirTopRight.y);
	shapeTopRight.setSize(Vector2f(1, 1));
	shapeTopRight.setFillColor(Color::Green);

	shapeBottomLeft.setPosition(posChangeDirBottomLeft.x, posChangeDirBottomLeft.y);
	shapeBottomLeft.setSize(Vector2f(1, 1));
	shapeBottomLeft.setFillColor(Color::Green);

	shapeBottomRight.setPosition(posChangeDirBottomRight.x, posChangeDirBottomRight.y);
	shapeBottomRight.setSize(Vector2f(1, 1));
	shapeBottomRight.setFillColor(Color::Green);
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
		widget.draw(shapeTopLeft);
		widget.draw(shapeTopRight);
		widget.draw(shapeBottomLeft);
		widget.draw(shapeBottomRight);
	);
}

Direzione Curva::getChangeDir(Vector2f pos)
{
	Vector2i posizione = Vector2i(pos.x, pos.y);
	switch (tipo)
	{
		case TipoBlocco::SX_TO_UP:
			if (posizione.x == posChangeDirBottomRight.x && posizione.y == posChangeDirBottomRight.y)
				return Direzione::SU;
			if (posizione.x == posChangeDirTopLeft.x && posizione.y == posChangeDirTopLeft.y)
				return Direzione::SX;
			break;
		case TipoBlocco::SX_TO_DOWN:
			if (posizione.x == posChangeDirBottomLeft.x && posizione.y == posChangeDirBottomLeft.y)
				return Direzione::GIU;
			if (posizione.x == posChangeDirTopRight.x && posizione.y == posChangeDirTopRight.y)
				return Direzione::SX;
			break;
		case TipoBlocco::DX_TO_UP:
			if (posizione.x == posChangeDirTopRight.x && posizione.y == posChangeDirTopRight.y)
				return Direzione::SU;
			if (posizione.x == posChangeDirBottomLeft.x && posizione.y == posChangeDirBottomLeft.y)
				return Direzione::DX;
			break;
		case TipoBlocco::DX_TO_DOWN:
			if (posizione.x == posChangeDirTopLeft.x && posizione.y == posChangeDirTopLeft.y)
				return Direzione::GIU;
			if (posizione.x == posChangeDirBottomRight.x && posizione.y == posChangeDirBottomRight.y)
				return Direzione::DX;
			break;
		default:;
	}
	return Direzione::ND;
}
