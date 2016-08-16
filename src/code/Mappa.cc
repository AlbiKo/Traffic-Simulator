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

	//Bordo sinistro
	generateSource(0, 0, true);

	//Bordo in alto
	generateSource(0, 0, false);

	//Bordo a destra
	generateSource(blocchiX - 1, 0, true);

	//Bordo in basso
	generateSource(0, blocchiY - 1, false);
}

void Mappa::generateSource(int x, int y, bool vertical)
{ 
	int min			= 1,											//Indice minimo del bordo
		max			= vertical ? blocchiY - 2 : blocchiX - 2,		//Indice massimo del bordo
		max_source	= max / 3,										//Numero massimo di sorgenti
		start_pos	= rand() % max + min,							//Posizione iniziale da cui partire per inserire le sorgenti
		i			= start_pos,									//Indice attuale
		count		= 0;											//Numero di sorgenti posizionate

	//Per completare il calcolo del numero di sorgenti massime
	if (max % 3)
		max_source++;

	do {
		if (i==start_pos || randomBool())
		{
			if (vertical)
			{
				if (i==max || (isEmptyBlock(blocchi[i + 1][x]->getTipo()) && isEmptyBlock(blocchi[i + 2][x]->getTipo())) )
				{					
					cambiaTipoBlocco(blocchi[i][x], TipoBlocco::HORIZONTAL);

					D2(PRINT("Sorgente in " <<x <<", " <<i));
					sorgenti.insert(Vector2i(x, i));
					count++;
					i+=2;
				}
			}
			else if (i==max || (isEmptyBlock(blocchi[y][i + 1]->getTipo()) && isEmptyBlock(blocchi[y][i + 2]->getTipo())) )
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

	} while (i != start_pos && count<max_source); //Finché non si torna alla posizione iniziale o si posizionano il numero di sorgenti stabilite
}

void Mappa::deleteSource(Vector2i source)
{
	Vector2i temp = sorgenti.get(source.x, source.y, true);

	//Se c'era effetivamente sulla lista
	if (temp.x == source.x && temp.y == source.y)
		cambiaTipoBlocco(blocchi[source.y][source.x], TipoBlocco::EMPTY);
}

void Mappa::checkUnlinkedSources(Vector2i_List& starts, Vector2i_List& ends)
{
	//Controllo delle partenze
	for (int i = 0; i < starts.count(); i++)
	{
		Vector2i startPos = starts.get(i, false);
		if ((startPos.x == 0 && isEmptyBlock(blocchi[startPos.y][startPos.x + 1]->getTipo())) ||
			(startPos.x == blocchiX - 1 && isEmptyBlock(blocchi[startPos.y][startPos.x - 1]->getTipo())) )
		{
			//Se non riesce a creare la strada
			if (!generateRoute(startPos, getNearestSource(startPos)))
				deleteSource(startPos);
		}
	}

	//Controllo delle destinazioni
	for (int i = 0; i < ends.count(); i++)
	{
		Vector2i endPos = ends.get(i, false);
		if ((endPos.y == 0 && isEmptyBlock(blocchi[endPos.y + 1][endPos.x]->getTipo())) ||
			(endPos.y == blocchiY - 1 && isEmptyBlock(blocchi[endPos.y - 1][endPos.x]->getTipo())) )
		{
			//Se non riesce a creare la strada
			if (!generateRoute(getNearestSource(endPos), endPos))
				deleteSource(endPos);
		}
	}
}

//C'è del codice ripetuto, magari migliorare
Vector2i Mappa::getNearestSource(Vector2i source)
{
	Vector2i temp = Vector2i(-1, -1);

	int fixedCoord = 0;

	//Sorgente a sinistra
	if (source.x == 0)
	{
		fixedCoord = (source.y < blocchiY / 2) ? 0 : blocchiY - 1;

		for (int i = 1; i < blocchiX - 1; i++)
		{
			temp = sorgenti.get(i, fixedCoord, false); 
			if (temp != Vector2i(-1, -1))
				return temp;
		}
	}

	//Sorgente sopra
	if (source.y == 0)
	{
		fixedCoord = (source.x < blocchiX / 2) ? 0 : blocchiX - 1;

		for (int i = 1; i < blocchiY - 1; i++)
		{
			temp = sorgenti.get(fixedCoord, i, false); 
			if (temp != Vector2i(-1, -1))
				return temp;
		}
	}

	//Sorgente a destra
	if (source.x == blocchiX - 1)
	{
		fixedCoord = (source.y < blocchiY / 2) ? 0 : blocchiY - 1;

		for (int i = blocchiX - 2; i > 0; i--)
		{
			temp = sorgenti.get(i, fixedCoord, false); 
			if (temp != Vector2i(-1, -1))
				return temp;
		}
	}

	//Sorgente sotto
	if (source.y == blocchiY - 1)
	{
		fixedCoord = (source.x < blocchiX / 2) ? 0 : blocchiX - 1;

		for (int i = blocchiY - 2; i > 0; i--)
		{
			temp = sorgenti.get(fixedCoord, i, false); 
			if (temp != Vector2i(-1, -1))
				return temp;
		}
	}

	return Vector2i(-1,-1);
}

void Mappa::generateRoutes()
{
	D1(PRINT("Generazione strade.."));
	assert(sorgenti.count() >= 4);

	Vector2i_List	partenze		= Vector2i_List(),
					destinazioni	= Vector2i_List();

	//Le sorgenti vengono suddivise in due liste:
	//partenze, quelle a lato
	//destinazioni, quelle sopra e sotto
	for (int i = 0; i < sorgenti.count(); i++)
	{
		Vector2i sorg = sorgenti.get(i, false);

		if (sorg.x == 0 || sorg.x == blocchiX - 1)
			partenze.insert(sorg);

		if (sorg.y == 0 || sorg.y == blocchiY - 1)
			destinazioni.insert(sorg);
	}

	//Si collegano le partenze con una destinazione scelta in modo casuale
	for (int i = 0; i < partenze.count(); i++)
		generateRoute(partenze.get(i, false), destinazioni.get(rand() % destinazioni.count(), false));

	checkUnlinkedSources(partenze, destinazioni);
}

bool Mappa::generateRoute(Vector2i startPos, Vector2i endPos)
{
	D1(PRINT("\n---------------\nGenerazione strada.."));
	D1(PRINT("Sorgente iniziale " << startPos.x << ", " << startPos.y));
	D1(PRINT("Sorgente finale " << endPos.x << ", " << endPos.y));
	assert(startPos.x >= 0 && startPos.y >= 0 && startPos.x < blocchiX && startPos.y < blocchiY);
	assert(endPos.x >= 0 && endPos.y >= 0 && endPos.x < blocchiX && endPos.y < blocchiY);
	//Numero di tentativi rimasti per la costruzione della strada
	int tentativi = MAX_TENTATIVI;
	Direzionatore dir = Direzionatore();

	//Il percorso come lista di blocchi
	Blocco_List bloccoList = Blocco_List();

	//Direzione estratta precedente
	Direzione prevDir = Direzione::ND;

	//Ultimo blocco piazzato
	TipoBlocco prevBlock = TipoBlocco::EMPTY;
	Vector2i currentPos(0,0);
	
	initGeneratingRoute(startPos, currentPos, prevDir, prevBlock);
	
	D1(PRINT("Posizione corrente " << currentPos.x << ", " << currentPos.y));
	D1(PRINT("Posizione finale " << endPos.x << ", " << endPos.y));
	D1(PRINT("PrevDir " <<stampaDir(prevDir)));
	
	do
	{
		dir.escludiDirezioni(currentPos, endPos, prevDir, Vector2i(blocchiX, blocchiY));

		Direzione currentDir = Direzione::ND;

		//Tipo del blocco da inserire
		TipoBlocco tipo = TipoBlocco::EMPTY;

		D1(PRINT("Controllo incroci"));
		do
		{
			dir.escludiDirezione(currentDir);
			currentDir = dir.estraiDirezione();
			tipo = mergeRouteBlocks(currentPos, prevDir, currentDir);

		} while ((checkAdjacentCross(currentPos, tipo)				||		//Si controllano conflitti con gli incroci sulla matrice
				  checkAdjacentCross(bloccoList, currentPos, tipo)	||		//Si controllano conflitti con gli incroci sul percorso temporaneo
				  checkZigZag(prevBlock, tipo)						) &&	
				 dir.count() != 0);											//Se c'è ancora almeno una direzione percorribile

		//Se non ci sono più direzioni percorribi si azzera il percorso e 
		//si diminuiscono il numero di tentativi rimasti
		if (dir.count() == 0)
		{
			bloccoList.clean();
			tentativi--;
			D1(PRINT("Fallimento strada.. tentativi rimasti: " <<tentativi));
			initGeneratingRoute(startPos, currentPos, prevDir, prevBlock);
		} 
		else
		{
			//Si percorre la direzione estratta
			prevBlock = tipo;
			nextStepRouteBlock(bloccoList, currentPos, prevDir, currentDir, tipo);
		}

	} while ((currentPos.x != endPos.x || currentPos.y != endPos.y) && tentativi != 0);

	if (tentativi == 0)
		return false;

	//Si applica il percorso
	applyRouteBlocks(bloccoList);
	return true;
}

void Mappa::initGeneratingRoute(Vector2i startPos, Vector2i & currentPos, Direzione & prevDir, TipoBlocco& prevBlock)
{
	prevBlock = TipoBlocco::HORIZONTAL;

	if (startPos.x == 0 && startPos.y > 0 && startPos.y < blocchiY - 1)
	{
		//Si sta partendo da una sorgente sul lato sinistro,
		//quindi il blocco da posizionare è quello immediatamente a destra..
		currentPos = Vector2i(startPos.x + 1, startPos.y);

		//.. e quindi ci si è spostati verso destra
		prevDir = Direzione::DX;
	}
	else
		if (startPos.x == blocchiX - 1 && startPos.y > 0 && startPos.y < blocchiY - 1)
		{
			//Si sta partendo da una sorgente sul lato destro,
			//quindi il blocco da posizionare è quello immediatamente a sinistra..
			currentPos = Vector2i(startPos.x - 1, startPos.y);

			//.. e quindi ci si è spostati verso sinistra
			prevDir = Direzione::SX;
		}
}

void Mappa::nextStepRouteBlock(Blocco_List& bloccoList, Vector2i & currentPos, Direzione & prevDir, Direzione currentDir, TipoBlocco tipo)
{
	D1(PRINT("Inserimento blocco in lista.."));
	D2(PRINT("CurrentPos "<<currentPos.x <<", " <<currentPos.y));
	D2(PRINT("CurrentDir " <<stampaDir(currentDir)));
	D2(PRINT("Tipo --------------> " <<stampaTipoBlocco(tipo)));

	bloccoList.insert(Blocco(currentPos.y, currentPos.x, tipo));
	
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

void Mappa::applyRouteBlocks(Blocco_List & bloccoList)
{
	D1(PRINT("\n************\nApplicazione blocchi"));
	for (int i = 0; i < bloccoList.count(); i++)
	{
		Blocco b = bloccoList.get(i, false);
		Vector2i coord = b.coordBlocco();
		D2(PRINT("\nPosizione " << coord.x << ", " << coord.y));
		assert(coord.x >= 0 && coord.y >= 0 && coord.x < blocchiX && coord.y < blocchiY);
		D2(PRINT("Tipo " << stampaTipoBlocco(b.getTipo())));
		cambiaTipoBlocco(blocchi[coord.y][coord.x], b.getTipo());
	}
}

bool Mappa::checkAdjacentCross(Vector2i currentPos, TipoBlocco tipo)
{
	if (!isCrossBlock(tipo))
		return false;

		//Controlla ai lati
	if ((isCrossBlock(blocchi[currentPos.y + 1][currentPos.x]->getTipo())) ||
		(isCrossBlock(blocchi[currentPos.y - 1][currentPos.x]->getTipo())) ||
		(isCrossBlock(blocchi[currentPos.y][currentPos.x + 1]->getTipo())) ||
		(isCrossBlock(blocchi[currentPos.y][currentPos.x - 1]->getTipo())) ||
		
		//Controllo agli angoli
		(isCrossBlock(blocchi[currentPos.y + 1][currentPos.x - 1]->getTipo())) ||
		(isCrossBlock(blocchi[currentPos.y + 1][currentPos.x + 1]->getTipo())) ||
		(isCrossBlock(blocchi[currentPos.y - 1][currentPos.x + 1]->getTipo())) ||
		(isCrossBlock(blocchi[currentPos.y - 1][currentPos.x - 1]->getTipo())) )
			return true;

	return false;
}

bool Mappa::checkAdjacentCross(Blocco_List& bloccoList, Vector2i currentPos, TipoBlocco tipo)
{
	if (!isCrossBlock(tipo))
		return false;

		//Controllo ai lati
	if ((isCrossBlock(bloccoList.get(currentPos.x + 1, currentPos.y, false).getTipo())) ||
		(isCrossBlock(bloccoList.get(currentPos.x - 1, currentPos.y, false).getTipo())) ||
		(isCrossBlock(bloccoList.get(currentPos.x, currentPos.y + 1, false).getTipo())) ||
		(isCrossBlock(bloccoList.get(currentPos.x, currentPos.y - 1, false).getTipo())) ||

		//Controllo agli angoli
		(isCrossBlock(bloccoList.get(currentPos.x + 1, currentPos.y - 1, false).getTipo())) ||
		(isCrossBlock(bloccoList.get(currentPos.x - 1, currentPos.y - 1, false).getTipo())) ||
		(isCrossBlock(bloccoList.get(currentPos.x + 1, currentPos.y + 1, false).getTipo())) ||
		(isCrossBlock(bloccoList.get(currentPos.x - 1, currentPos.y + 1, false).getTipo())) )
		return true;

	return false;
}

bool Mappa::checkZigZag(TipoBlocco prevBlock, TipoBlocco currentBlock)
{
	if (!isCurveBlock(prevBlock) || !isCurveBlock(currentBlock))
		return false;

	switch (prevBlock)
	{
	case TipoBlocco::SX_TO_UP:
		if (currentBlock == TipoBlocco::DX_TO_DOWN)
			return true;
		break;
	case TipoBlocco::SX_TO_DOWN:
		if (currentBlock == TipoBlocco::DX_TO_UP)
			return true;
		break;
	case TipoBlocco::DX_TO_UP:
		if (currentBlock == TipoBlocco::SX_TO_DOWN)
			return true;
		break;
	case TipoBlocco::DX_TO_DOWN:
		if (currentBlock == TipoBlocco::SX_TO_UP)
			return true;
		break;
	default:
		break;
	}

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

	D1(PRINT("Posizionerei blocco di tipo ------> " <<stampaTipoBlocco(tipo)));
	
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

	if (isRectBlock(tipo))
	{
		blocco = new Rettilineo(coord.y, coord.x, tipo);
		return;
	}

	if (isCurveBlock(tipo))
	{
		blocco = new Curva(coord.y, coord.x, tipo);
		return;
	}

	if (isCross3Block(tipo))
	{
		blocco = new Incrocio3(coord.y, coord.x, tipo);
		return;
	}

	if (isCross4Block(tipo))
	{
		blocco = new Incrocio4(coord.y, coord.x);
		return;
	}

	blocco = new Blocco(coord.y, coord.x, tipo);
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
	sorgenti.clean();

	for (int i = 0; i < blocchiY; i++)
		for (int j = 0; j < blocchiX; j++)
			cambiaTipoBlocco(blocchi[i][j], TipoBlocco::EMPTY);
}

