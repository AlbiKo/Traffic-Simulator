#ifndef TIPOBLOCCO_INCLUDE
	#define TIPOBLOCCO_INCLUDE

	/**Enum che rappresenta la/e direzione/i dei vari tipi di blocco.
	*/
	enum class TipoBlocco
	{
		HORIZONTAL = 0, VERTICAL,
		SX_TO_UP, SX_TO_DOWN, DX_TO_UP, DX_TO_DOWN,
		CROSS3_SX, CROSS3_DX, CROSS3_UP, CROSS3_DOWN,
		CROSS4,
		EMPTY
	};
#endif
