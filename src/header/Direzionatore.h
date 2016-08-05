#ifndef DIREZIONATORE_INCLUDE
#define DIREZIONATORE_INCLUDE

#include "Direzione.h"
#include "Direzione_List.h"
#include "enum_toInt.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>


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
	void escludiDirezioni(sf::Vector2i partenza, sf::Vector2i destinazione, Direzione prevDir, sf::Vector2i numBlocchi);
	void ripristina();

	inline int count()
	{
		return dl.count();
	}
private:
	Direzione_List dl;
};

#endif // !DIREZIONATORE_INCLUDE