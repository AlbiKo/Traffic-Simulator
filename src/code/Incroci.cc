#include "../header/Incroci.h"
#include "../header/Debug.h"

Direzione Incroci::getChangeDir(Vector2f pos, Direzione dir)
{
	Vector2i posizione = Vector2i(pos.x, pos.y);
	switch (dir)
	{
		case Direzione::SU:
			if (posizione == posChangeDirTopLeft)
				return Direzione::DX;
			if ((posizione == posChangeDirTopRight) || (posizione == posChangeDirBottomRight))
				return Direzione::SU;
			break;
		case Direzione::GIU:
			if (posizione == posChangeDirBottomRight)
				return Direzione::SX;
			if ((posizione == posChangeDirTopLeft) || (posizione == posChangeDirBottomLeft))
				return Direzione::GIU;
			break;
		case Direzione::DX:
			if (posizione == posChangeDirTopRight)
				return Direzione::GIU;
			if ((posizione == posChangeDirBottomRight) || (posizione == posChangeDirBottomLeft))
				return Direzione::DX;
			break;
		case Direzione::SX:
			if (posizione == posChangeDirBottomLeft)
				return Direzione::SU;
			if ((posizione == posChangeDirTopLeft) || (posizione == posChangeDirTopRight))
				return Direzione::SX;
			break;
		default:
			break;
	}
	return Direzione::ND;
}

void Incroci::setChangeDirPos()
{
	Vector2i pos = getPos();
	int secondaryOffset = size - offsetChangeDirPos - 1;
	
	posChangeDirTopLeft.x = offsetChangeDirPos + pos.x;
	posChangeDirTopLeft.y = offsetChangeDirPos + pos.y;

	posChangeDirTopRight.x = secondaryOffset + pos.x;
	posChangeDirTopRight.y = offsetChangeDirPos + pos.y;

	posChangeDirBottomLeft.x = offsetChangeDirPos + pos.x;
	posChangeDirBottomLeft.y = secondaryOffset + pos.y;

	posChangeDirBottomRight.x = secondaryOffset + pos.x;
	posChangeDirBottomRight.y = secondaryOffset + pos.y;
}

void Incroci::initSemShapes()
{
	initSemShape(semShape1, posChangeDirTopLeft);
	initSemShape(semShape2, posChangeDirTopRight);
	initSemShape(semShape3, posChangeDirBottomLeft);
	initSemShape(semShape4, posChangeDirBottomRight);

	setSemaphoresColor();
}

void Incroci::initSemShape(CircleShape &semShape, Vector2i pos)
{
	int SEM_RADIUS = 5;

	semShape.setRadius(SEM_RADIUS);
	semShape.setOrigin(SEM_RADIUS, SEM_RADIUS);
	semShape.setPosition(pos.x, pos.y);
}

void Incroci::setSemaphoresColor()
{
	Color horizColor = Color::Red, vertColor = Color::Green;

	if (!blockHoriz)
	{
		horizColor = Color::Green;
		vertColor = Color::Red;
	}

	semShape1.setFillColor(vertColor);
	semShape2.setFillColor(horizColor);
	semShape3.setFillColor(horizColor);
	semShape4.setFillColor(vertColor);
}

void Incroci::setSemaphorePos()
{
	Vector2i pos = getPos();

	if (!blockHoriz)
	{
		semaphore1.left = posChangeDirTopLeft.x;
		semaphore1.top = pos.y;

		semaphore2.left = posChangeDirBottomRight.x;
		semaphore2.top = pos.y + Blocco::size;
	}
	else
	{
		semaphore1.left = pos.x;
		semaphore1.top = posChangeDirBottomLeft.y;

		semaphore2.left = pos.x + Blocco::size;
		semaphore2.top = posChangeDirTopRight.y;
	}
}

void Incroci::changeSemaphoreStatus()
{
	blockHoriz = !blockHoriz;

	setSemaphorePos();
	setSemaphoresColor();
}

IntRect Incroci::getSemaphore(Direzione dir)
{
	switch (dir)
	{
	case Direzione::SU:
	case Direzione::SX:
		return semaphore2;
	case Direzione::GIU:
	case Direzione::DX:
		return semaphore1;
	case Direzione::ND:
	default:
		return IntRect();
	}
}
