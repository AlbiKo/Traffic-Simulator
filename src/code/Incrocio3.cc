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
	shape_1.setPosition(pos1.x, pos1.y);
	shape_1.setSize(Vector2f(1, 1));
	shape_1.setFillColor(Color::White);

	shape_2.setPosition(pos2.x, pos2.y);
	shape_2.setSize(Vector2f(1, 1));
	shape_2.setFillColor(Color::White);

	shape_3.setPosition(pos3.x, pos3.y);
	shape_3.setSize(Vector2f(1, 1));
	shape_3.setFillColor(Color::White);

	shape_4.setPosition(pos4.x, pos4.y);
	shape_4.setSize(Vector2f(1, 1));
	shape_4.setFillColor(Color::White);
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
		widget.draw(shape_1);
		widget.draw(shape_2);
		widget.draw(shape_3);
		widget.draw(shape_4);
	);
}
