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


std::string toString(Direzione d)
{
	switch (d)
	{
	case Direzione::SU:
		return "Su";

	case Direzione::GIU:
		return "Giu";

	case Direzione::DX:
		return "Destra";

	case Direzione::SX:
		return "Sinistra";

	case Direzione::ND:
	default:
		return "Non definita";
	}
}

