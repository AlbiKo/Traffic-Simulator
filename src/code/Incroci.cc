#include "../header/Incroci.h"
#include "../header/Debug.h"

/*Incroci::Incroci()
{
}*/

/*Incroci::Incroci(int rowIndex, int columnIndex)
{
}*/

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

void Incroci::setPos(int rowIndex, int columnIndex)
{
	pos1.x = 19 + (size*columnIndex);
	pos1.y = 19 + (size*rowIndex);

	pos2.x = 48 + (size*columnIndex);
	pos2.y = 19 + (size*rowIndex);

	pos3.x = 19 + (size*columnIndex);
	pos3.y = 48 + (size*rowIndex);

	pos4.x = 48 + (size*columnIndex);
	pos4.y = 48 + (size*rowIndex);
}


