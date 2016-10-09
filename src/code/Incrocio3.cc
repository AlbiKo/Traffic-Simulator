#include "../header/Incrocio3.h"
#include "../header/Debug.h"

Incrocio3::Incrocio3()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::CROSS3_UP);
	wrongDir = Direzione::GIU;
	setSemaphorePos();
	initSemShapes();
}

Incrocio3::Incrocio3(int rowIndex, int columnIndex, TipoBlocco tipo)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	collider = IntRect(columnIndex * size, rowIndex * size, size, size);

	cambiaVerso(tipo);

	setChangeDirPos();
	setSemaphorePos();
	initSemShapes();

#ifdef DEBUG_MODE
	shapeTopLeft.setPosition(posChangeDirTopLeft.x, posChangeDirTopLeft.y);
	shapeTopLeft.setSize(Vector2f(1, 1));
	shapeTopLeft.setFillColor(Color::White);

	shapeTopRight.setPosition(posChangeDirTopRight.x, posChangeDirTopRight.y);
	shapeTopRight.setSize(Vector2f(1, 1));
	shapeTopRight.setFillColor(Color::White);

	shapeBottomLeft.setPosition(posChangeDirBottomLeft.x, posChangeDirBottomLeft.y);
	shapeBottomLeft.setSize(Vector2f(1, 1));
	shapeBottomLeft.setFillColor(Color::White);

	shapeBottomRight.setPosition(posChangeDirBottomRight.x, posChangeDirBottomRight.y);
	shapeBottomRight.setSize(Vector2f(1, 1));
	shapeBottomRight.setFillColor(Color::White);
#endif // DEBUG_MDOE	
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

void Incrocio3::draw(RenderWindow & widget)
{
	widget.draw(sprite);

	if (wrongDir != Direzione::SU)
		widget.draw(semShape1);

	if (wrongDir != Direzione::DX)
		widget.draw(semShape2);

	if (wrongDir != Direzione::SX)
		widget.draw(semShape3);

	if (wrongDir != Direzione::GIU)
		widget.draw(semShape4);

	D1(
		widget.draw(shapeTopLeft);
		widget.draw(shapeTopRight);
		widget.draw(shapeBottomLeft);
		widget.draw(shapeBottomRight);
	);
}
