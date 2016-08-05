#ifndef DIREZIONE_INCLUDE
#define DIREZIONE_INCLUDE

enum class Direzione
{
	ND = 0, SU, GIU, DX, SX
};

Direzione getDirOpposta(Direzione d);


#endif // !DIREZIONE_INCLUDE