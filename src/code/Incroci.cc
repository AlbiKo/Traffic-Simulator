#include "../header/Incroci.h"
#include "../header/Debug.h"

Direzione Incroci::getChangeDir(Vector2f pos, Direzione dir)
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

void Incroci::setChangeDirPos()
{
	Vector2i pos = getPos();

	pos1.x = 19 + pos.x;
	pos1.y = 19 + pos.y;

	pos2.x = 48 + pos.x;
	pos2.y = 19 + pos.y;

	pos3.x = 19 + pos.x;
	pos3.y = 48 + pos.y;

	pos4.x = 48 + pos.x;
	pos4.y = 48 + pos.y;
}

void Incroci::setSemaphorePos()
{
	Vector2i pos = getPos();

	if (blockHoriz)
	{
		semaphore1.left = pos1.x;
		semaphore1.top = pos.y;

		semaphore2.left = pos4.x;
		semaphore2.top = pos.y + Blocco::size;
	}
	else
	{
		semaphore1.left = pos.x;
		semaphore1.top = pos3.y;

		semaphore2.left = pos.x + Blocco::size;
		semaphore2.top = pos2.y;
	}
}

void Incroci::changeSemaphoreStatus()
{
	blockHoriz = !blockHoriz;

	setSemaphorePos();
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
