#include <SFML/Graphics.hpp>

using namespace sf;

class Nodo
{

private:
    Vector2i pos;

    Nodo * adiacenze[4];
    int numAdiacenze;

public:

	int peso;

    Nodo();
    Nodo(Vector2i startPos);

	void setPos(int x, int y);

	Nodo * getAdiacenza(int i);
	void setAdiacenza(int i, Nodo * nodo);
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
