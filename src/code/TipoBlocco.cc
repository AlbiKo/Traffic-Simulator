#include "../header/TipoBlocco.h"

bool isRectBlock(TipoBlocco tipo)
{
	if (tipo == TipoBlocco::HORIZONTAL || tipo == TipoBlocco::VERTICAL)
		return true;

	return false;
}

bool isCurveBlock(TipoBlocco tipo)
{
	if (tipo == TipoBlocco::SX_TO_DOWN || tipo == TipoBlocco::SX_TO_UP ||
		tipo == TipoBlocco::DX_TO_DOWN || tipo == TipoBlocco::DX_TO_UP)
		return true;

	return false;
}

bool isCrossBlock(TipoBlocco tipo)
{
	if (isCross3Block(tipo) || isCross4Block(tipo))
		return true;

	return false;
}

bool isCross3Block(TipoBlocco tipo)
{
	if (tipo == TipoBlocco::CROSS3_DOWN || tipo == TipoBlocco::CROSS3_DX ||
		tipo == TipoBlocco::CROSS3_UP || tipo == TipoBlocco::CROSS3_SX)
		return true;

	return false;
}

bool isCross4Block(TipoBlocco tipo)
{
	return (tipo == TipoBlocco::CROSS4) ? true :false;
}

bool isEmptyBlock(TipoBlocco tipo)
{
	return (tipo == TipoBlocco::EMPTY) ? true : false;
}

std::string stampaTipoBlocco(TipoBlocco tipo)
{
	switch (tipo)
	{
		//Blocchi rettilineo
	case TipoBlocco::HORIZONTAL:
		return "Rettilineo orizzontale";
	case TipoBlocco::VERTICAL:
		return "Rettilineo verticale";

		//Blocchi curva
	case TipoBlocco::SX_TO_UP:
		return "Curva da sinistra verso alto";
	case TipoBlocco::SX_TO_DOWN:
		return "Curva da sinistra verso basso";
	case TipoBlocco::DX_TO_UP:
		return "Curva da destra verso alto";
	case TipoBlocco::DX_TO_DOWN:
		return "Curva da destra verso basso";

		//Blocchi incrocio a 3
	case TipoBlocco::CROSS3_SX:
		return "Incrocio3 con terza uscita a sinistra";
	case TipoBlocco::CROSS3_DX:
		return "Incrocio3 con terza uscita a destra";
	case TipoBlocco::CROSS3_UP:
		return "Incrocio3 con terza uscita in alto";
	case TipoBlocco::CROSS3_DOWN:
		return "Incrocio3 con terza uscita in basso";

		//Blocco incrocio a 4
	case TipoBlocco::CROSS4:
		return "Incrocio4";

		//Blocco vuoto
	case TipoBlocco::EMPTY:
		return "Blocco vuoto";
	default:
		return "";
	}
}

