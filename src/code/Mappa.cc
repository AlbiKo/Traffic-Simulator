#include "../header/Mappa.h"
#include "../header/Debug.h"

extern int RESX, RESY;
using namespace sf;

Texture * texture[11];

Mappa::Mappa()
{
	loadTextures();

	blocchiX = RESX / Blocco::size;
	if (RESX % Blocco::size != 0)
		blocchiX++;

	D1(PRINT("Totale blocchi su X " <<blocchiX));

	blocchiY = RESY / Blocco::size;
	if (RESY % Blocco::size != 0)
		blocchiY++;

	D1(PRINT("Totale blocchi su Y " <<blocchiY));

	blocchi = new Blocco**[blocchiY];

	for (int i = 0; i < blocchiY; i++)
	{	blocchi[i] = new Blocco*[blocchiX];
		for (int j = 0; j < blocchiX; j++)
			blocchi[i][j] = new Blocco(i, j, VUOTO);
	}


	cambiaTipoBlocco(blocchi[0][0], CROSS3_SX);
	cambiaTipoBlocco(blocchi[1][0], CROSS3_DX);

	Rettilineo * ret = dynamic_cast<Rettilineo *>(blocchi[0][0]);
	if (ret != NULL)
		ret->cambiaVerso(ORIZZONTALE);
}

void Mappa::generate()
{
	D1(PRINT("Genero"));
}

void Mappa::draw(RenderWindow &widget)
{
	for (int i = 0; i < blocchiY; i++)
		for (int j = 0; j < blocchiX; j++)
			if (blocchi[i][j]->getTipo() != VUOTO)
				widget.draw(blocchi[i][j]->getSprite());
}

void Mappa::cambiaTipoBlocco(Blocco * &blocco, TipoBlocco tipo)
{
	Vector2i coord = blocco->coordBlocco();
	delete blocco;

	if (tipo == VERTICALE || tipo == ORIZZONTALE)
	{
		blocco = new Rettilineo(coord.y, coord.x, tipo);
		return;
	}

	if (tipo == SX_TO_DOWN || tipo == DX_TO_DOWN || tipo == SX_TO_UP || tipo ==	DX_TO_UP)
	{
		blocco = new Curva(coord.y, coord.x, tipo);
		return;
	}

	if (tipo == CROSS3_DOWN || tipo == CROSS3_UP || tipo == CROSS3_DX || tipo == CROSS3_SX)
	{
		blocco = new Incrocio3(coord.y, coord.x, tipo);
		return;
	}

	if (tipo == CROSS4)
	{
		blocco = new Incrocio4(coord.y, coord.x);
		return;
	}

	if (tipo == VUOTO)
	{
		blocco = new Blocco(coord.y, coord.x, tipo);
		return;
	}
}

void Mappa::loadTextures()
{
	for (int i = 0; i < 11; i++)
		texture[i] = new Texture();

	assert(texture[ORIZZONTALE]->loadFromFile("media/img/orizzontale.jpg"));
	if (!texture[ORIZZONTALE]->loadFromFile("media/img/orizzontale.jpg"))
		exit(1);

	assert(texture[VERTICALE]->loadFromFile("media/img/verticale.jpg"));
	if (!texture[VERTICALE]->loadFromFile("media/img/verticale.jpg"))
		exit(1);

	assert(texture[SX_TO_UP]->loadFromFile("media/img/curvaSxSu.jpg"));
	if (!texture[SX_TO_UP]->loadFromFile("media/img/curvaSxSu.jpg"))
		exit(1);

	assert(texture[SX_TO_DOWN]->loadFromFile("media/img/curvaSxGiu.jpg"));
	if (!texture[SX_TO_DOWN]->loadFromFile("media/img/curvaSxGiu.jpg"))
		exit(1);

	assert(texture[DX_TO_UP]->loadFromFile("media/img/curvaDxSu.jpg"));
	if (!texture[DX_TO_UP]->loadFromFile("media/img/curvaDxSu.jpg"))
		exit(1);

	assert(texture[DX_TO_DOWN]->loadFromFile("media/img/curvaDxGiu.jpg"));
	if (!texture[DX_TO_DOWN]->loadFromFile("media/img/curvaDxGiu.jpg"))
		exit(1);

	assert(texture[CROSS3_UP]->loadFromFile("media/img/incrocio3su.jpg"));
	if (!texture[CROSS3_UP]->loadFromFile("media/img/incrocio3su.jpg"))
		exit(1);

	assert(texture[CROSS3_DOWN]->loadFromFile("media/img/incrocio3giu.jpg"));
	if (!texture[CROSS3_DOWN]->loadFromFile("media/img/incrocio3giu.jpg"))
		exit(1);

	assert(texture[CROSS3_DX]->loadFromFile("media/img/incrocio3dx.jpg"));
	if (!texture[CROSS3_DX]->loadFromFile("media/img/incrocio3dx.jpg"))
		exit(1);

	assert(texture[CROSS3_SX]->loadFromFile("media/img/incrocio3sx.jpg"));
	if (!texture[CROSS3_SX]->loadFromFile("media/img/incrocio3sx.jpg"))
		exit(1);

	assert(texture[CROSS4]->loadFromFile("media/img/incrocio4.jpg"));
	if (!texture[CROSS4]->loadFromFile("media/img/incrocio4.jpg"))
		exit(1);
}
