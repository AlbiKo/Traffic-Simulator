#ifndef DIREZIONATORE_INCLUDE
#define DIREZIONATORE_INCLUDE

#include "Direzione.h"
#include "Direzione_List.h"
#include "enum_toInt.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

using namespace sf;
/**
	Costituisce una sorta di interfaccia alla lista di Direzioni, 
	regolando in modo leggermente diverso le operazioni della lista.

	La lista non avrà mai più di quattro elementi (le quattro direzioni), i quali sono tutti distinti
	Si aggiunge la possibilità di estrarre casualmente un elemento della lista
*/
class Direzionatore
{
public:
	Direzionatore();
	~Direzionatore();

	Direzione estraiDirezione();
	void ammettiDirezione(Direzione d);
	void escludiDirezione(Direzione d);
	void escludiDirezioni(Vector2i startPos, Vector2i endPos, Direzione prevDir, Vector2i numBlocchi);
	void ripristina();

	inline int count()
	{
		return dl.count();
	}
private:
	Direzione_List dl;
	void escludiDirezioniX(Vector2i startPos, Vector2i endPos, Vector2i border, Direzione oppositeDir);
};

#endif // DIREZIONATORE_INCLUDE