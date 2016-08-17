#ifndef DIREZIONE_INCLUDE
#define DIREZIONE_INCLUDE
#include <iostream>

/** Rappresenta le quattro direzioni percorribili e, in aggiunta, anche la direzione nulla (ND = Non Definita). */
enum class Direzione
{
	ND = 0, SU, GIU, DX, SX
};

/** Individua, se possibile, la direzione opposta alla direzione passata come parametro.
*
*	@param d Direzione
*	@return La direzione opposta se possibile. Altrimenti la direzione nulla
*/
Direzione getDirOpposta(Direzione d);

/** Stringhizza la direzione passata come parametro.
*
*	@param d Direzione
*	@return Stringa che rappresenta la direzione
*/
std::string toString(Direzione d);

#endif // DIREZIONE_INCLUDE