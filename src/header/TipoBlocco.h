#ifndef TIPOBLOCCO_INCLUDE
	#define TIPOBLOCCO_INCLUDE
	#include <iostream>
	enum class TipoBlocco
	{
		HORIZONTAL = 0, VERTICAL,
		SX_TO_UP, SX_TO_DOWN, DX_TO_UP, DX_TO_DOWN,
		CROSS3_SX, CROSS3_DX, CROSS3_UP, CROSS3_DOWN,
		CROSS4,
		EMPTY
	};

	bool isRectBlock(TipoBlocco tipo);
	bool isCurveBlock(TipoBlocco tipo);
	bool isCross3Block(TipoBlocco tipo);
	bool isCross4Block(TipoBlocco tipo);
	bool isEmptyBlock(TipoBlocco tipo);

	std::string stampaTipoBlocco(TipoBlocco tipo);
#endif
