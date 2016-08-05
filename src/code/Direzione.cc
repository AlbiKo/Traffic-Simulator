#include "../header/Direzione.h"

Direzione getDirOpposta(Direzione d)
{
	switch (d)
	{
	case Direzione::SU:
		return Direzione::GIU;

	case Direzione::GIU:
		return Direzione::SU;

	case Direzione::DX:
		return Direzione::SX;

	case Direzione::SX:
		return Direzione::DX;

	case Direzione::ND:
	default:
		return Direzione::ND;
	}
}