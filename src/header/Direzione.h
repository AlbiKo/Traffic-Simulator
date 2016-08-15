#ifndef DIREZIONE_INCLUDE
#define DIREZIONE_INCLUDE
#include <iostream>
enum class Direzione
{
	ND = 0, SU, GIU, DX, SX
};

Direzione getDirOpposta(Direzione d);

std::string stampaDir(Direzione d);

#endif // DIREZIONE_INCLUDE