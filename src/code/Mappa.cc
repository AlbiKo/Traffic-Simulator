#include "../header/Mappa.h"
/**
	Mantiene la matrice dei blocchi

	creazione sorgenti
	step successivo partendo da un certo blocco

	Serve direzioni

*/

using namespace sf;
Sprite * sprite[11];
Blocco * blocco;

Mappa::Mappa()
{
	loadTextures();
	for (int i = 0; i < 11; i++)
	{
		sprite[i] = new Sprite();
		sprite[i]->setPosition(i*68, 0);
		sprite[i]->setTexture(texture[i]);
	}
	//int ** a = new int [3][4];
	blocco = new Blocco(1, 1, ORIZZONTALE, &texture[ORIZZONTALE]);
}

void Mappa::draw(RenderWindow &widget)
{
	for (int i = 0; i < 11; i++)
		widget.draw(*sprite[i]);

	widget.draw(blocco->getSprite());
}

void Mappa::loadTextures()
{
	assert(texture[ORIZZONTALE].loadFromFile("media/img/orizzontale.jpg"));
	if (!texture[ORIZZONTALE].loadFromFile("media/img/orizzontale.jpg"))
		exit(1);

	assert(texture[VERTICALE].loadFromFile("media/img/verticale.jpg"));
	if (!texture[VERTICALE].loadFromFile("media/img/verticale.jpg"))
		exit(1);

	assert(texture[SX_TO_UP].loadFromFile("media/img/curvaSxSu.jpg"));
	if (!texture[SX_TO_UP].loadFromFile("media/img/curvaSxSu.jpg"))
		exit(1);

	assert(texture[SX_TO_DOWN].loadFromFile("media/img/curvaSxGiu.jpg"));
	if (!texture[SX_TO_DOWN].loadFromFile("media/img/curvaSxGiu.jpg"))
		exit(1);

	assert(texture[DX_TO_UP].loadFromFile("media/img/curvaDxSu.jpg"));
	if (!texture[DX_TO_UP].loadFromFile("media/img/curvaDxSu.jpg"))
		exit(1);

	assert(texture[DX_TO_DOWN].loadFromFile("media/img/curvaDxGiu.jpg"));
	if (!texture[DX_TO_DOWN].loadFromFile("media/img/curvaDxGiu.jpg"))
		exit(1);

	assert(texture[CROSS3_UP].loadFromFile("media/img/incrocio3su.jpg"));
	if (!texture[CROSS3_UP].loadFromFile("media/img/incrocio3su.jpg"))
		exit(1);

	assert(texture[CROSS3_DOWN].loadFromFile("media/img/incrocio3giu.jpg"));
	if (!texture[CROSS3_DOWN].loadFromFile("media/img/incrocio3giu.jpg"))
		exit(1);

	assert(texture[CROSS3_DX].loadFromFile("media/img/incrocio3dx.jpg"));
	if (!texture[CROSS3_DX].loadFromFile("media/img/incrocio3dx.jpg"))
		exit(1);

	assert(texture[CROSS3_SX].loadFromFile("media/img/incrocio3sx.jpg"));
	if (!texture[CROSS3_SX].loadFromFile("media/img/incrocio3sx.jpg"))
		exit(1);

	assert(texture[CROSS4].loadFromFile("media/img/incrocio4.jpg"));
	if (!texture[CROSS4].loadFromFile("media/img/incrocio4.jpg"))
		exit(1);
}
