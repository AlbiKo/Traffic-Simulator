#include <SFML/Graphics.hpp>

using namespace sf;

/** Classe nodo bella
	bella.
	eh
*/

class Nodo
{

private:
    Vector2i pos;

	struct Adiacenza
	{
		Nodo * nodo;
		int peso;
	} adiacenze[4];

    int numAdiacenze;

	void setAdiacenza(int i, Nodo * nodo);

public:

    Nodo();
    Nodo(Vector2i startPos);

	void setPos(int x, int y);

	Nodo * getAdiacenza(int i, int &peso);

	bool setAdiacenza(Nodo * nodo);

	inline int getNumAdiacenze()
	{
		return numAdiacenze;
	}

	inline Vector2i getPos()
	{
		return pos;
	}
};
