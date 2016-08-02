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
			blocchi[i][j] = new Blocco(i, j, TipoBlocco::EMPTY);
	}


	cambiaTipoBlocco(blocchi[0][0], TipoBlocco::CROSS3_SX);
	cambiaTipoBlocco(blocchi[1][0], TipoBlocco::CROSS3_DX);

	Rettilineo * ret = dynamic_cast<Rettilineo *>(blocchi[0][0]);
	if (ret != NULL)
		ret->cambiaVerso(TipoBlocco::HORIZONTAL);
}

void Mappa::generate()
{
	D1(PRINT("Genero"));
}

void Mappa::draw(RenderWindow &widget)
{
	for (int i = 0; i < blocchiY; i++)
		for (int j = 0; j < blocchiX; j++)
			if (blocchi[i][j]->getTipo() != TipoBlocco::EMPTY)
				widget.draw(blocchi[i][j]->getSprite());
}

void Mappa::cambiaTipoBlocco(Blocco * &blocco, TipoBlocco tipo)
{
	Vector2i coord = blocco->coordBlocco();
	delete blocco;

	if (tipo == TipoBlocco::VERTICAL || tipo == TipoBlocco::HORIZONTAL)
	{
		blocco = new Rettilineo(coord.y, coord.x, tipo);
		return;
	}

	if (tipo == TipoBlocco::SX_TO_DOWN || tipo == TipoBlocco::DX_TO_DOWN || tipo == TipoBlocco::SX_TO_UP || tipo ==	TipoBlocco::DX_TO_UP)
	{
		blocco = new Curva(coord.y, coord.x, tipo);
		return;
	}

	if (tipo == TipoBlocco::CROSS3_DOWN || tipo == TipoBlocco::CROSS3_UP || tipo == TipoBlocco::CROSS3_DX || tipo == TipoBlocco::CROSS3_SX)
	{
		blocco = new Incrocio3(coord.y, coord.x, tipo);
		return;
	}

	if (tipo == TipoBlocco::CROSS4)
	{
		blocco = new Incrocio4(coord.y, coord.x);
		return;
	}

	if (tipo == TipoBlocco::EMPTY)
	{
		blocco = new Blocco(coord.y, coord.x, tipo);
		return;
	}
}

void Mappa::loadTextures()
{
	for (int i = 0; i < 11; i++)
		texture[i] = new Texture();

	assert(texture[toInt(TipoBlocco::HORIZONTAL)]->loadFromFile("media/img/orizzontale.jpg"));
	if (!texture[toInt(TipoBlocco::HORIZONTAL)]->loadFromFile("media/img/orizzontale.jpg"))
		exit(1);

	assert(texture[toInt(TipoBlocco::VERTICAL)]->loadFromFile("media/img/verticale.jpg"));
	if (!texture[toInt(TipoBlocco::VERTICAL)]->loadFromFile("media/img/verticale.jpg"))
		exit(1);

	assert(texture[toInt(TipoBlocco::SX_TO_UP)]->loadFromFile("media/img/curvaSxSu.jpg"));
	if (!texture[toInt(TipoBlocco::SX_TO_UP)]->loadFromFile("media/img/curvaSxSu.jpg"))
		exit(1);

	assert(texture[toInt(TipoBlocco::SX_TO_DOWN)]->loadFromFile("media/img/curvaSxGiu.jpg"));
	if (!texture[toInt(TipoBlocco::SX_TO_DOWN)]->loadFromFile("media/img/curvaSxGiu.jpg"))
		exit(1);

	assert(texture[toInt(TipoBlocco::DX_TO_UP)]->loadFromFile("media/img/curvaDxSu.jpg"));
	if (!texture[toInt(TipoBlocco::DX_TO_UP)]->loadFromFile("media/img/curvaDxSu.jpg"))
		exit(1);

	assert(texture[toInt(TipoBlocco::DX_TO_DOWN)]->loadFromFile("media/img/curvaDxGiu.jpg"));
	if (!texture[toInt(TipoBlocco::DX_TO_DOWN)]->loadFromFile("media/img/curvaDxGiu.jpg"))
		exit(1);

	assert(texture[toInt(TipoBlocco::CROSS3_UP)]->loadFromFile("media/img/incrocio3su.jpg"));
	if (!texture[toInt(TipoBlocco::CROSS3_UP)]->loadFromFile("media/img/incrocio3su.jpg"))
		exit(1);

	assert(texture[toInt(TipoBlocco::CROSS3_DOWN)]->loadFromFile("media/img/incrocio3giu.jpg"));
	if (!texture[toInt(TipoBlocco::CROSS3_DOWN)]->loadFromFile("media/img/incrocio3giu.jpg"))
		exit(1);

	assert(texture[toInt(TipoBlocco::CROSS3_DX)]->loadFromFile("media/img/incrocio3dx.jpg"));
	if (!texture[toInt(TipoBlocco::CROSS3_DX)]->loadFromFile("media/img/incrocio3dx.jpg"))
		exit(1);

	assert(texture[toInt(TipoBlocco::CROSS3_SX)]->loadFromFile("media/img/incrocio3sx.jpg"));
	if (!texture[toInt(TipoBlocco::CROSS3_SX)]->loadFromFile("media/img/incrocio3sx.jpg"))
		exit(1);

	assert(texture[toInt(TipoBlocco::CROSS4)]->loadFromFile("media/img/incrocio4.jpg"));
	if (!texture[toInt(TipoBlocco::CROSS4)]->loadFromFile("media/img/incrocio4.jpg"))
		exit(1);
}
