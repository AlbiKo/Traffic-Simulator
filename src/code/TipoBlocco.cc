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
