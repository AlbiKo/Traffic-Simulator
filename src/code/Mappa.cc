#include "../header/Mappa.h"
#include "../header/Debug.h"

extern int RESX, RESY;
using namespace sf;

Texture * texture[11];

Mappa::Mappa()
{
	loadTextures();
	sorgenti = Vector2i_List();

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

	/*
	Rettilineo * ret = dynamic_cast<Rettilineo *>(blocchi[0][0]);
	if (ret != NULL)
		ret->cambiaVerso(TipoBlocco::HORIZONTAL);
	*/
}

void Mappa::generate()
{
	D1(PRINT("\n\n\n\n\n\nGenerazione mappa.."));
	srand(time(NULL));
	clean();
	generateSources();
	generateRoutes();

}

void Mappa::generateSources()
{
	D1(PRINT("Generazione sorgenti.."));

	sorgenti.clean();

	generateSource(0, 0, true);
	generateSource(0, 0, false);
	generateSource(blocchiX - 1, 0, true);
	generateSource(0, blocchiY - 1, false);
}

void Mappa::generateSource(int x, int y, bool vertical) //vertical=false->horizontal
{ 
	int min			= 1, 
		max			= vertical ? blocchiY - 2 : blocchiX - 2,
		max_source	= max / 3,
		start_pos	= rand() % max + min,
		i			= start_pos, 
		count		= 0;

	//Per completare il calcolo del numero di sorgenti massime
	if (max % 3)
		max_source++;

	do {
		if (i==start_pos || randomBool())
		{
			if (vertical)
			{
				if (i==max || (blocchi[i + 1][x]->getTipo() == TipoBlocco::EMPTY && blocchi[i + 2][x]->getTipo() == TipoBlocco::EMPTY))
				{					
					cambiaTipoBlocco(blocchi[i][x], TipoBlocco::HORIZONTAL);

					D2(PRINT("Sorgente in " <<x <<", " <<i));
					sorgenti.insert(Vector2i(x, i));
					count++;
					i+=2;
				}
			}
			else if (i==max || (blocchi[y][i + 1]->getTipo() == TipoBlocco::EMPTY && blocchi[y][i + 2]->getTipo() == TipoBlocco::EMPTY))
				{
					cambiaTipoBlocco(blocchi[y][i], TipoBlocco::VERTICAL);

					D2(PRINT("Sorgente in " << i << ", " << y));
					sorgenti.insert(Vector2i(i, y));
					count++;
					i+=2;
				}
		}

		i++;
		if (i > max)
			i = min;

	} while (i != start_pos && count<max_source);
}


void Mappa::generateRoutes()
{
	D1(PRINT("Generazione strade.."));
	assert(sorgenti.count() >= 4);

	Vector2i vez = sorgenti.get(0, false);
	generateRoute(vez, sorgenti.get(sorgenti.count()-1, false));
}

//DA FARE: non si può compiere più di un zigghezagghe
void Mappa::generateRoute(Vector2i startPos, Vector2i endPos)
{
	Direzionatore dir = Direzionatore();

	Direzione prevDir = Direzione::ND;
	Vector2i currentPos(0,0);
	initGeneratingRoute(startPos, endPos, currentPos, prevDir);
	
	if (prevDir == Direzione::ND)
		return;

	D1(PRINT("\nStartPos" << startPos.x << ", " << startPos.y));
	D1(PRINT("CurrentPos " << currentPos.x << ", " << currentPos.y));
	D1(PRINT("PrevDir " << toInt(prevDir)));
	
	do {
		dir.escludiDirezioni(currentPos, endPos, prevDir, Vector2i(blocchiX, blocchiY));
	//	Direzione currentDir;

		//Non è possibile procedere verso alcuna direzione dal punto in cui ci si trova
		if (dir.count() == 0)
			return;


		if ((currentPos.y == 1 && endPos.y == 1) || (currentPos.y == blocchiY - 2 && endPos.y == blocchiY - 2))
		{
			autocompleteRoute(currentPos, endPos, prevDir);
			return;
		}
		applyRouteBlock(currentPos, prevDir, dir.estraiDirezione(), TipoBlocco::HORIZONTAL);
	} while (currentPos.x != endPos.x || currentPos.y != endPos.y);

	autocompleteRoute(currentPos, endPos, prevDir);
}

void Mappa::initGeneratingRoute(Vector2i startPos, Vector2i& endPos, Vector2i & currentPos, Direzione & prevDir)
{
	if (endPos.y == 0)
		endPos.y++;
	else
		endPos.y--;

	if (startPos.x == 0 && startPos.y > 0 && startPos.y < blocchiY - 1)
	{
		//Si sta partendo da una sorgente sul lato sinistro,
		//quindi il blocco da posizionare è quello immediatamente a destra..
		currentPos = Vector2i(startPos.x + 1, startPos.y);

		//.. e quindi non si può tornare verso il blocco precente (la sorgente)
		// cioè verso sinistra
		prevDir = Direzione::SX;
	}
	else
	{
		if (startPos.x == blocchiX - 1 && startPos.y > 0 && startPos.y < blocchiY - 1)
		{
			//Si sta partendo da una sorgente sul lato destro,
			//quindi il blocco da posizionare è quello immediatamente a sinistra..
			currentPos = Vector2i(startPos.x - 1, startPos.y);

			//.. e quindi non si può tornare verso il blocco precente (la sorgente)
			// cioè verso destra
			prevDir = Direzione::DX;
		}
	}
}

void Mappa::applyRouteBlock(Vector2i & currentPos, Direzione & prevDir, Direzione currentDir, TipoBlocco tipo)
{
	D1(PRINT("CurrentPos "<<currentPos.x <<", " <<currentPos.y));
	D1(PRINT("CurrentDir " <<toInt(currentDir)));
	cambiaTipoBlocco(blocchi[currentPos.y][currentPos.x], tipo);
	prevDir = getDirOpposta(currentDir);
	D1(PRINT("PrevDir " << toInt(prevDir)));
	switch (currentDir)
	{
	case Direzione::SU:
		currentPos.y--;
		break;

	case Direzione::GIU:
		currentPos.y++;
		break;

	case Direzione::DX:
		currentPos.x++;
		break;

	case Direzione::SX:
		currentPos.x--;
		break;

	case Direzione::ND:
	default:
		return;
	}
	D1(PRINT("Nuova CurrentPos " << currentPos.x << ", " << currentPos.y));
}

void Mappa::autocompleteRoute(Vector2i currentPos, Vector2i endPos, Direzione prevDir)
{
	D1(PRINT("Autocompletamento.."));
	//Se ci si ritrova esattamente sulla destinazione e si viene dal basso/alto
	if (currentPos.x == endPos.x && 
		(currentPos.y == 1 || currentPos.y == blocchiY - 2) &&
		(prevDir == Direzione::GIU || prevDir == Direzione::SU))
	{
		cambiaTipoBlocco(blocchi[currentPos.y][currentPos.x], TipoBlocco::VERTICAL);
		return;
	}

	//Dove si trova la destinazione rispetto alla pos corrente
	Direzione pos; 
	//Coefficiente di avvicinamento alla destinazione
	int coeff = 1;
	if (currentPos.x > endPos.x)
	{
		pos = Direzione::SX;
		coeff = -1;
	}
	else
		pos = Direzione::DX;

	//Ci si avvicina alla destinazione
	while (currentPos.x != endPos.x)
	{
		cambiaTipoBlocco(blocchi[currentPos.y][currentPos.x], TipoBlocco::HORIZONTAL);
		currentPos.x += coeff;
	}

	if (currentPos.y == 1)
	{
		if (pos == Direzione::SX)
			cambiaTipoBlocco(blocchi[currentPos.y][currentPos.x], TipoBlocco::DX_TO_UP);
		else
			cambiaTipoBlocco(blocchi[currentPos.y][currentPos.x], TipoBlocco::SX_TO_UP);
	}
	else if (currentPos.y == blocchiY - 2)
	{
		if (pos == Direzione::SX)
			cambiaTipoBlocco(blocchi[currentPos.y][currentPos.x], TipoBlocco::DX_TO_DOWN);
		else
			cambiaTipoBlocco(blocchi[currentPos.y][currentPos.x], TipoBlocco::SX_TO_DOWN);

	}
}

void Mappa::mergeRouteBlocks(Vector2i pos, Direzione prevDir, Direzione currentDir)
{
}

bool Mappa::randomBool()
{
	return (rand() % RAND_MAX) % 2;
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

void Mappa::clean()
{
	for (int i = 0; i < blocchiY; i++)
		for (int j = 0; j < blocchiX; j++)
			cambiaTipoBlocco(blocchi[i][j], TipoBlocco::EMPTY);
}

