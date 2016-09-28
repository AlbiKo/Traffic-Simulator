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
		widget.draw(shape_1);
		widget.draw(shape_2);
		widget.draw(shape_3);
		widget.draw(shape_4);
	);
}