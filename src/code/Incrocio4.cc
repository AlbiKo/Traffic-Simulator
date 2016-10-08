#include "../header/Incrocio4.h"
#include "../header/Debug.h"

Incrocio4::Incrocio4()
{
	sprite.setPosition(0,0);
	setTipo(TipoBlocco::CROSS4);
	setSemaphorePos();
	initSemShapes();
}

Incrocio4::Incrocio4(int rowIndex, int columnIndex)
{
	sprite.setPosition(columnIndex * size, rowIndex * size);
	collider = IntRect(columnIndex * size, rowIndex * size, size, size);
	setTipo(TipoBlocco::CROSS4);

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
#endif // DEBUG_MODE
}

void Incrocio4::draw(RenderWindow & widget)
{
	widget.draw(sprite);

	widget.draw(semShape1);
	widget.draw(semShape2);
	widget.draw(semShape3);
	widget.draw(semShape4);

	D1(
		widget.draw(shapeTopLeft);
		widget.draw(shapeTopRight);
		widget.draw(shapeBottomLeft);
		widget.draw(shapeBottomRight);
	);
}