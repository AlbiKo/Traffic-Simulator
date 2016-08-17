#ifndef MACCHINA_INCLUDE
#define MACCHINA_INCLUDE

#include <SFML/Graphics.hpp>
#include "Direzione.h"
#include "Blocco_List.h"
using namespace sf;

class Macchina
{
public:
	const int SIZE = 15;

	IntRect colliderDx,
			colliderSx,
			colliderUp,
			colliderDown;

	

	Macchina();
	Macchina(Vector2i pos);
	
	~Macchina();
	void draw(RenderWindow &widget);
	void setPosition(Vector2i pos);
	void setColor(Color c);
	RectangleShape getShape();

	bool isMoving();
	bool changeDirection(Direzione dir);
	void stop();
	void update();
	Direzione getDirection();
	
private:
	RectangleShape shape;
	Blocco_List percorso;
	Direzione moving;

	void initMacchina();
	void updateColliders();

};

#endif
