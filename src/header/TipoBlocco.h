#ifndef TIPOBLOCCO_INCLUDE
	#define TIPOBLOCCO_INCLUDE
	#include <iostream>

	/** Rappresenta i tipi che un blocco può assumere con eventuali orientamenti. */
	enum class TipoBlocco
	{
		HORIZONTAL = 0, VERTICAL,						//Rettilinei
		SX_TO_UP, SX_TO_DOWN, DX_TO_UP, DX_TO_DOWN,		//Curve
		CROSS3_SX, CROSS3_DX, CROSS3_UP, CROSS3_DOWN,	//Incrocio a tre vie. La direzione indicata alla fine, indica dove si trova la via di immissione.
		CROSS4,											//Incrocio a quattro vie
		EMPTY											//Blocco vuoto
	};

	/** Stabilisce se il tipo di blocco passato come parametro appartiene alla categoria dei rettilinei.
	*
	*	@param tipo Tipo del blocco.
	*	@return True se appartiene alla categoria. False altrimenti.
	*/
	bool isRectBlock(TipoBlocco tipo);

	/** Stabilisce se il tipo di blocco passato come parametro appartiene alla categoria delle curve.
	*
	*	@param tipo Tipo del blocco.
	*	@return True se appartiene alla categoria. False altrimenti.
	*/
	bool isCurveBlock(TipoBlocco tipo);

	/** Stabilisce se il tipo di blocco passato come parametro appartiene alla categoria degli incroci (a tre e a quattro intersezioni compresi).
	*
	*	@param tipo Tipo del blocco.
	*	@return True se appartiene alla categoria. False altrimenti.
	*/
	bool isCrossBlock(TipoBlocco tipo);

	/** Stabilisce se il tipo di blocco passato come parametro appartiene alla categoria degli incroci a tre intersezioni.
	*
	*	@param tipo Tipo del blocco.
	*	@return True se appartiene alla categoria. False altrimenti.
	*/
	bool isCross3Block(TipoBlocco tipo);

	/** Stabilisce se il tipo di blocco passato come parametro appartiene alla categoria dell'incrocio a quattro intersezioni.
	*
	*	@param tipo Tipo del blocco.
	*	@return True se appartiene alla categoria. False altrimenti.
	*/
	bool isCross4Block(TipoBlocco tipo);

	/** Stabilisce se il tipo di blocco passato come parametro appartiene alla categoria del blocco vuoto.
	*
	*	@param tipo Tipo del blocco.
	*	@return True se appartiene alla categoria. False altrimenti.
	*/
	bool isEmptyBlock(TipoBlocco tipo);

	/** Restituisce una stringa in italiano indicante il tipo di blocco passato come parametro.
	*
	*	@param tipo Tipo del blocco.
	*	@return La stringa che rappresenta il tipo di blocco.
	*/
	std::string toString(TipoBlocco tipo);
#endif
