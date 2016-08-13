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

//DA FARE: non si può mettere una sorgente nei primi due blocchi di un lato se c'è già nei primi due blocchi dell'altro lato (in senso orario)
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

	Vector2i_List	partenzeSx		= Vector2i_List(),
					destinazioniSx	= Vector2i_List(),
					partenzeDx		= Vector2i_List(),
					destinazioniDx	= Vector2i_List();

	for (int i = 0; i < sorgenti.count(); i++)
	{
		Vector2i sorg = sorgenti.get(i, false);

		if (sorg.x == 0)
			partenzeSx.insert(sorg);

		if (sorg.x == blocchiX - 1)
			partenzeDx.insert(sorg);

		if (sorg.x != 0 && sorg.x < blocchiX / 2)
			destinazioniSx.insert(sorg);

		if (sorg.x != blocchiX - 1 && sorg.x >= blocchiX / 2)
			destinazioniDx.insert(sorg);
	}

/*	for (int i = 0; i < partenzeSx.count(); i++)
		for (int j = 0; j < destinazioniSx.count(); j++)
			generateRoute(partenzeSx.get(i, false), destinazioniSx.get(j, false));

	for (int i = 0; i < partenzeDx.count(); i++)
		for (int j = 0; j < destinazioniDx.count(); j++)
			generateRoute(partenzeDx.get(i, false), destinazioniDx.get(j, false));*/
	for (int i = 0; i < partenzeSx.count(); i++)
			generateRoute(partenzeSx.get(i, false), destinazioniDx.get(rand() % destinazioniDx.count(), false));

	for (int i = 0; i < partenzeDx.count(); i++)
		generateRoute(partenzeDx.get(i, false), destinazioniSx.get(rand() % destinazioniSx.count(), false));
}

//DA FARE: non si può compiere più di un zigghezagghe
void Mappa::generateRoute(Vector2i startPos, Vector2i endPos)
{
	D1(PRINT("\n---------------\nGenerazione strada.."));
	D1(PRINT("Sorgente iniziale " << startPos.x << ", " << startPos.y));
	D1(PRINT("Sorgente finale " << endPos.x << ", " << endPos.y));

	Direzionatore dir = Direzionatore();
	Direzione prevDir = Direzione::ND;
	Vector2i currentPos(0,0);
	Blocco_List bloccoList = Blocco_List();

	int tentativi = 4;

	initGeneratingRoute(startPos, currentPos, prevDir);
	
	D1(PRINT("Posizione corrente " << currentPos.x << ", " << currentPos.y));
	D1(PRINT("Posizione finale " << endPos.x << ", " << endPos.y));
	D1(PRINT("PrevDir " <<stampaDir(prevDir)));
	
	do
	{
		dir.escludiDirezioni(currentPos, endPos, prevDir, Vector2i(blocchiX, blocchiY));

		//Non è possibile procedere verso alcuna direzione dal punto in cui ci si trova
		if (dir.count() == 0)
			break;

		Direzione currentDir = dir.estraiDirezione();
		TipoBlocco tipo = mergeRouteBlocks(currentPos, prevDir, currentDir);

		applyRouteBlock(currentPos, prevDir, currentDir, tipo);
	} while (currentPos.x != endPos.x || currentPos.y != endPos.y);

}

void Mappa::initGeneratingRoute(Vector2i startPos, Vector2i & currentPos, Direzione & prevDir)
{
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
	D2(PRINT("CurrentPos "<<currentPos.x <<", " <<currentPos.y));
	D2(PRINT("CurrentDir " <<stampaDir(currentDir)));
	D2(PRINT("Tipo " <<stampaTipoBlocco(tipo)));

	cambiaTipoBlocco(blocchi[currentPos.y][currentPos.x], tipo);
	
	D2(PRINT("PrevDir " << stampaDir(prevDir)));

	prevDir = currentDir;

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
	D3(PRINT("Nuova CurrentPos " << currentPos.x << ", " << currentPos.y));
}

bool Mappa::checkAdjacentCross(Vector2i currentPos, TipoBlocco tipo)
{
	if (!isCross3Block(tipo) && !isCross4Block(tipo))
		return false;

	if ((isCross3Block(blocchi[currentPos.y + 1][currentPos.x]->getTipo()) || isCross4Block(blocchi[currentPos.y + 1][currentPos.x]->getTipo())) ||
		(isCross3Block(blocchi[currentPos.y - 1][currentPos.x]->getTipo()) || isCross4Block(blocchi[currentPos.y - 1][currentPos.x]->getTipo())) ||
		(isCross3Block(blocchi[currentPos.y][currentPos.x + 1]->getTipo()) || isCross4Block(blocchi[currentPos.y][currentPos.x + 1]->getTipo())) ||
		(isCross3Block(blocchi[currentPos.y][currentPos.x - 1]->getTipo()) || isCross4Block(blocchi[currentPos.y][currentPos.x - 1]->getTipo())))
			return true;

	return false;
}

TipoBlocco Mappa::mergeRouteBlocks(Vector2i currentPos, Direzione prevDir, Direzione currentDir)
{
	TipoBlocco tipo = blocchi[currentPos.y][currentPos.x]->getTipo();
	switch (tipo)
	{
	//Blocchi rettilineo
	case TipoBlocco::HORIZONTAL:
		tipo = mergeRectRouteBlock(tipo, prevDir, currentDir, Direzione::SX, Direzione::SU, TipoBlocco::CROSS3_UP, TipoBlocco::CROSS3_DOWN);
		break;
	case TipoBlocco::VERTICAL:
		tipo = mergeRectRouteBlock(tipo, prevDir, currentDir, Direzione::SU, Direzione::SX, TipoBlocco::CROSS3_SX, TipoBlocco::CROSS3_DX);
		break;

	//Blocchi curva
	case TipoBlocco::SX_TO_UP:
		tipo = mergeCurveRouteBlock(tipo, prevDir, currentDir, Direzione::DX, Direzione::GIU, TipoBlocco::CROSS3_UP, TipoBlocco::CROSS3_SX);
		break;
	case TipoBlocco::SX_TO_DOWN:
		tipo = mergeCurveRouteBlock(tipo, prevDir, currentDir, Direzione::DX, Direzione::SU, TipoBlocco::CROSS3_DOWN, TipoBlocco::CROSS3_SX);
		break;
	case TipoBlocco::DX_TO_UP:
		tipo = mergeCurveRouteBlock(tipo, prevDir, currentDir, Direzione::SX, Direzione::GIU, TipoBlocco::CROSS3_UP, TipoBlocco::CROSS3_DX);
		break;
	case TipoBlocco::DX_TO_DOWN:
		tipo = mergeCurveRouteBlock(tipo, prevDir, currentDir, Direzione::SX, Direzione::SU, TipoBlocco::CROSS3_DOWN, TipoBlocco::CROSS3_DX);
		break;

	//Blocchi incrocio a 3
	case TipoBlocco::CROSS3_SX:
		tipo = mergeCross3RouteBlock(tipo, prevDir, currentDir, Direzione::DX);
		break;
	case TipoBlocco::CROSS3_DX:
		tipo = mergeCross3RouteBlock(tipo, prevDir, currentDir, Direzione::SX);
		break;
	case TipoBlocco::CROSS3_UP:
		tipo = mergeCross3RouteBlock(tipo, prevDir, currentDir, Direzione::GIU);
		break;
	case TipoBlocco::CROSS3_DOWN:
		tipo = mergeCross3RouteBlock(tipo, prevDir, currentDir, Direzione::SU);
		break;

	//Blocco vuoto
	case TipoBlocco::EMPTY:
		tipo = mergeEmptyRouteBlock(prevDir, currentDir);
		break;
	default:
		break;
	}

	//Controllo se il tipo del blocco attualmente stabilito va in conflitto con eventuali sorgenti
	tipo = checkSourceRouteBlock(currentPos, tipo);

	D1(PRINT("Posizionerei blocco di tipo: " <<stampaTipoBlocco(tipo)));
	
	return tipo;
}

TipoBlocco Mappa::mergeRectRouteBlock(TipoBlocco tipo, Direzione prevDir, Direzione currentDir, Direzione rectDir, Direzione perpDir, TipoBlocco perpBlock, TipoBlocco nonPerpBlock)
{
	//Si verifica che si stia lavorando su un rettilineo,
	//che perpDir sia perpendicolare a rectDir,
	//che non si torni indietro
	if (!isRectBlock(tipo) || 
		perpDir == rectDir || perpDir == getDirOpposta(rectDir) ||
		currentDir == getDirOpposta(prevDir))
		return tipo;

	if (prevDir == currentDir)
		//Se ci si sta muovendo secondo la direzione del rettilineo
		if (prevDir == rectDir || prevDir == getDirOpposta(rectDir))
			return tipo;
		else
			//Si è perpendicolari al rettilineo
			return TipoBlocco::CROSS4;
	else
		//Se si sta entrando nel rettilineo e poi in che senso
		if (currentDir == rectDir || currentDir == getDirOpposta(rectDir))
			return (prevDir == getDirOpposta(perpDir)) ? perpBlock : nonPerpBlock;
		else
			//Si sta uscendo dal rettilineo, da stabilire il senso
			return (currentDir == perpDir) ? perpBlock : nonPerpBlock;

	return tipo;
}

TipoBlocco Mappa::mergeCurveRouteBlock(TipoBlocco tipo, Direzione prevDir, Direzione currentDir, Direzione missingDirX, Direzione missingDirY, TipoBlocco typeX, TipoBlocco typeY)
{
	//Se si ripercorre la curva in uno dei due sensi di marcia
	if (!isCurveBlock(tipo) ||
		(prevDir == missingDirX && currentDir == getDirOpposta(missingDirY)) ||
		(prevDir == missingDirY && currentDir == getDirOpposta(missingDirX)))
		return tipo;

	//Se non si ripercorre nessuna parte della curva
	if ((prevDir == getDirOpposta(missingDirX) && currentDir == missingDirY) ||
		(prevDir == getDirOpposta(missingDirY) && currentDir == missingDirX))
		return TipoBlocco::CROSS4;
	
	//Se si forma un incrocio a 3 con il lato piatto sull'asse X
	if ((prevDir == missingDirY && currentDir == missingDirX) ||
		(prevDir == missingDirX && currentDir == missingDirX) ||
		(prevDir == getDirOpposta(missingDirX) && currentDir == getDirOpposta(missingDirX)) ||
		(prevDir == getDirOpposta(missingDirX) && currentDir == getDirOpposta(missingDirY)))
		return typeX;

	//Se si forma un incrocio a 3 con il lato piatto sull'asse X
	if ((prevDir == missingDirX && currentDir == missingDirY) ||
		(prevDir == missingDirY && currentDir == missingDirY) ||
		(prevDir == getDirOpposta(missingDirY) && currentDir == getDirOpposta(missingDirY)) ||
		(prevDir == getDirOpposta(missingDirY) && currentDir == getDirOpposta(missingDirX)))
		return typeY;

	return tipo;
}

TipoBlocco Mappa::mergeCross3RouteBlock(TipoBlocco tipo, Direzione prevDir, Direzione currentDir, Direzione missingDir)
{
	if (!isCross3Block(tipo))
		return tipo;

	if (prevDir == getDirOpposta(missingDir) || currentDir == missingDir)
		return TipoBlocco::CROSS4;

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
		//Se ci si trova sulla prima riga e sopra c'è una sorgente
		if (currentPos.y == 1 && sorgenti.get(currentPos.x, currentPos.y - 1, false) != Vector2i(-1, -1))
			tipo = TipoBlocco::CROSS3_UP;

		//Se ci si trova sull'ultima riga e sotto c'è una sorgente
		if (currentPos.y == blocchiY - 2 && sorgenti.get(currentPos.x, currentPos.y + 1, false) != Vector2i(-1, -1))
			tipo = TipoBlocco::CROSS3_DOWN;
		break;

	case TipoBlocco::VERTICAL:
		//Se ci si trova sulla prima colonna e a sinistra c'è una sorgente
		if (currentPos.x == 1 && sorgenti.get(currentPos.x - 1, currentPos.y, false) != Vector2i(-1, -1))
			tipo = TipoBlocco::CROSS3_SX;

		//Se ci si trova sull'ultima colonna e a destra c'è una sorgente
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
	//Si verifica che si stia controllando un blocco curva
	if (!isCurveBlock(base))
		return base;

	//Se il blocco si trova esattamente all'angolo
	if (currentPos.y == cornerPos.y && currentPos.x == cornerPos.x)
	{
		//Se a fianco si trova una sorgente
		if (sorgenti.get(currentPos.x + offset.x, currentPos.y, false) != Vector2i(-1, -1))
			//Se sopra/sotto si trova una sorgente
			if (sorgenti.get(currentPos.x, currentPos.y + offset.y, false) != Vector2i(-1, -1))
				return TipoBlocco::CROSS4;
			else
				return typeX;

		else if (sorgenti.get(currentPos.x, currentPos.y + offset.y, false) != Vector2i(-1, -1))
			return typeY;
	}
	else
	{
		//Se ci si trova nella stessa riga dell'angolo e se c'è una sorgente sopra/sotto
		if (currentPos.y == cornerPos.y && sorgenti.get(currentPos.x, currentPos.y + offset.y, false) != Vector2i(-1, -1))
			return typeY;
		else
			//Se ci si trova nella stessa colonna dell'angolo e se c'è una sorgente a fianco
			if (currentPos.x == cornerPos.x && sorgenti.get(currentPos.x + offset.x, currentPos.y, false) != Vector2i(-1, -1))
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

