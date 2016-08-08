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
	generateRoute(vez, sorgenti.get(3, false));
/*	generateRoute(vez, sorgenti.get(sorgenti.count() - 1, false));
	generateRoute(vez, sorgenti.get(sorgenti.count() - 1, false));*/
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
		

		//Non è possibile procedere verso alcuna direzione dal punto in cui ci si trova
		if (dir.count() == 0)
			return;


		if ((currentPos.y == 1 && endPos.y == 1) || (currentPos.y == blocchiY - 2 && endPos.y == blocchiY - 2))
		{
			autocompleteRoute(currentPos, endPos, prevDir);
			return;
		}
		Direzione currentDir = dir.estraiDirezione();
		applyRouteBlock(currentPos, prevDir, currentDir, mergeRouteBlocks(currentPos, prevDir, currentDir));
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

		//.. e quindi ci si è spostati verso destra
		prevDir = Direzione::DX;
	}
	else
	{
		if (startPos.x == blocchiX - 1 && startPos.y > 0 && startPos.y < blocchiY - 1)
		{
			//Si sta partendo da una sorgente sul lato destro,
			//quindi il blocco da posizionare è quello immediatamente a sinistra..
			currentPos = Vector2i(startPos.x - 1, startPos.y);

			//.. e quindi ci si è spostati verso sinistra
			prevDir = Direzione::SX;
		}
	}
}

void Mappa::applyRouteBlock(Vector2i & currentPos, Direzione & prevDir, Direzione currentDir, TipoBlocco tipo)
{
	D1(PRINT("Applico blocco.."));
	D1(PRINT("CurrentPos "<<currentPos.x <<", " <<currentPos.y));
	D1(PRINT("CurrentDir " <<toInt(currentDir)));
	D1(PRINT("Tipo " << toInt(tipo)));
	cambiaTipoBlocco(blocchi[currentPos.y][currentPos.x], tipo);
	prevDir = currentDir;

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
	Direzione dirPos; 
	//Coefficiente di avvicinamento alla destinazione
	int coeff = 1;
	if (currentPos.x > endPos.x)
	{
		dirPos = Direzione::SX;
		coeff = -1;
	}
	else
		dirPos = Direzione::DX;

	
	Direzione check = prevDir;
	//Ci si avvicina alla destinazione
	while (currentPos.x != endPos.x)
	{
		cambiaTipoBlocco(blocchi[currentPos.y][currentPos.x], mergeRouteBlocks(currentPos, check, dirPos));
		currentPos.x += coeff;

		check = dirPos;
	}

	Direzione currentDir = (currentPos.y == 1) ? Direzione::SU : Direzione::GIU;

	//Siamo sulla destinazione
	cambiaTipoBlocco(blocchi[currentPos.y][currentPos.x], mergeRouteBlocks(currentPos, dirPos, currentDir));
}

TipoBlocco Mappa::mergeRouteBlocks(Vector2i currentPos, Direzione prevDir, Direzione currentDir)
{
	TipoBlocco tipo = blocchi[currentPos.y][currentPos.x]->getTipo();
	switch (tipo)
	{
	case TipoBlocco::HORIZONTAL:
		break;
	case TipoBlocco::VERTICAL:
		break;
	case TipoBlocco::SX_TO_UP:
		break;
	case TipoBlocco::SX_TO_DOWN:
		break;
	case TipoBlocco::DX_TO_UP:
		break;
	case TipoBlocco::DX_TO_DOWN:
		break;
	case TipoBlocco::CROSS3_SX:
		break;
	case TipoBlocco::CROSS3_DX:
		break;
	case TipoBlocco::CROSS3_UP:
		break;
	case TipoBlocco::CROSS3_DOWN:
		break;
	case TipoBlocco::EMPTY:
		tipo = mergeEmptyRouteBlock(prevDir, currentDir);
		break;
	default:
		break;
	}

	tipo = checkSourceRouteBlock(currentPos, tipo);
	return tipo;
}

TipoBlocco Mappa::mergeEmptyRouteBlock(Direzione prevDir, Direzione currentDir)
{
	TipoBlocco tipo = TipoBlocco::EMPTY;

	switch (prevDir)
	{
	case Direzione::GIU:
		tipo = (currentDir == Direzione::SX) ? TipoBlocco::SX_TO_UP : TipoBlocco::DX_TO_UP;
		if (currentDir == Direzione::GIU)
			tipo = TipoBlocco::VERTICAL;
		break;
	case Direzione::SU:
		tipo = (currentDir == Direzione::SX) ? TipoBlocco::SX_TO_DOWN : TipoBlocco::DX_TO_DOWN;
		if (currentDir == Direzione::SU)
			tipo = TipoBlocco::VERTICAL;
		break;
	case Direzione::SX:
		tipo = (currentDir == Direzione::SU) ? TipoBlocco::DX_TO_UP : TipoBlocco::DX_TO_DOWN;
		if (currentDir == Direzione::SX)
			tipo = TipoBlocco::HORIZONTAL;
		break;
	case Direzione::DX:
		tipo = (currentDir == Direzione::SU) ? TipoBlocco::SX_TO_UP : TipoBlocco::SX_TO_DOWN;
		if (currentDir == Direzione::DX)
			tipo = TipoBlocco::HORIZONTAL;
		break;
	default:
		break;
	}

	return tipo;
}

TipoBlocco Mappa::checkSourceRouteBlock(Vector2i currentPos, TipoBlocco tipo)
{
	//Si controlla se è un blocco che può essere adiacente ad una sorgente
	if (currentPos.y != 1 && currentPos.y != blocchiY - 2 &&
		currentPos.x != 1 && currentPos.x != blocchiX - 2)
		return tipo;

	switch (tipo)
	{
	case TipoBlocco::HORIZONTAL:
		if (currentPos.y == 1 && sorgenti.get(currentPos.x, currentPos.y - 1, false) != Vector2i(-1, -1))
			tipo = TipoBlocco::CROSS3_UP;

		if (currentPos.y == blocchiY - 2 && sorgenti.get(currentPos.x, currentPos.y + 1, false) != Vector2i(-1, -1))
			tipo = TipoBlocco::CROSS3_DOWN;
		break;

	case TipoBlocco::VERTICAL:
		if (currentPos.x == 1 && sorgenti.get(currentPos.x - 1, currentPos.y, false) != Vector2i(-1, -1))
			tipo = TipoBlocco::CROSS3_SX;

		if (currentPos.x == blocchiX - 2 && sorgenti.get(currentPos.x + 1, currentPos.y, false) != Vector2i(-1, -1))
			tipo = TipoBlocco::CROSS3_DX;
		break;

	case TipoBlocco::SX_TO_UP:
		tipo = checkSourceCurveRouteBlock(currentPos, Vector2i(blocchiX - 2, blocchiY - 2), Vector2i(1, 1), tipo, TipoBlocco::CROSS3_UP, TipoBlocco::CROSS3_SX);
		break;

	case TipoBlocco::SX_TO_DOWN:
		tipo = checkSourceCurveRouteBlock(currentPos, Vector2i(blocchiX - 2, 1), Vector2i(1, -1), tipo, TipoBlocco::CROSS3_DOWN, TipoBlocco::CROSS3_SX);
		break;

	case TipoBlocco::DX_TO_UP:
		tipo = checkSourceCurveRouteBlock(currentPos, Vector2i(1, blocchiY - 2), Vector2i(-1, 1), tipo, TipoBlocco::CROSS3_UP, TipoBlocco::CROSS3_DX);
		break;

	case TipoBlocco::DX_TO_DOWN:
		tipo = checkSourceCurveRouteBlock(currentPos, Vector2i(1, 1), Vector2i(-1, -1), tipo, TipoBlocco::CROSS3_DOWN, TipoBlocco::CROSS3_DX);
		break;

	default:
		break;
	}
	return tipo;
}


TipoBlocco Mappa::checkSourceCurveRouteBlock(Vector2i currentPos, Vector2i cornerPos, Vector2i offset, TipoBlocco base, TipoBlocco typeX, TipoBlocco typeY)
{
	if (currentPos.y == cornerPos.y && currentPos.x == cornerPos.x)
	{
		if (sorgenti.get(currentPos.x + offset.x, currentPos.y, false) != Vector2i(-1, -1))
			if (sorgenti.get(currentPos.x, currentPos.y + offset.y, false) != Vector2i(-1, -1))
				return TipoBlocco::CROSS4;
			else
				return typeX;
		else if (sorgenti.get(currentPos.x, currentPos.y + offset.y, false) != Vector2i(-1, -1))
			return typeY;
	}
	else
	{
		if (currentPos.y == cornerPos.y && sorgenti.get(currentPos.x, currentPos.y + offset.y, false) != Vector2i(-1, -1))
			return typeY;
		else if (currentPos.x == cornerPos.x && sorgenti.get(currentPos.x + offset.x, currentPos.y, false) != Vector2i(-1, -1))
			return typeX;
	}

	return base;
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
	if (tipo == blocco->getTipo())
		return;

	Vector2i coord = blocco->coordBlocco();
	delete blocco;

	switch (tipo)
	{
	case TipoBlocco::HORIZONTAL:
	case TipoBlocco::VERTICAL:
		blocco = new Rettilineo(coord.y, coord.x, tipo);
		break;

	case TipoBlocco::SX_TO_UP:
	case TipoBlocco::SX_TO_DOWN:
	case TipoBlocco::DX_TO_UP:
	case TipoBlocco::DX_TO_DOWN:
		blocco = new Curva(coord.y, coord.x, tipo);
		break;

	case TipoBlocco::CROSS3_SX:
	case TipoBlocco::CROSS3_DX:
	case TipoBlocco::CROSS3_UP:
	case TipoBlocco::CROSS3_DOWN:
		blocco = new Incrocio3(coord.y, coord.x, tipo);
		break;

	case TipoBlocco::CROSS4:
		blocco = new Incrocio4(coord.y, coord.x);
		break;

	case TipoBlocco::EMPTY:
	default:
		blocco = new Blocco(coord.y, coord.x, tipo);
		break;
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

