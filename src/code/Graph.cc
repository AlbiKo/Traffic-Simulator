#include "../header/Graph.h"
#include "../header/Debug.h"
Graph::Graph()
{
	mapSize = Vector2i(0, 0);
}

Graph::~Graph()
{
	nodes.clean();
}

void Graph::buildGraph(Mappa &map)
{
	D1(PRINT("\n\n****************\nCostruzione grafo da mappa\n"));

	nodes.clean();
	mapSize = map.getMapSize();
	
	//Usato per numerare i nodi e poterli distinguere quando si stampano i parent.
	int count = 0;

	Vector2i_List sources;
	map.getSorgenti(sources);
	
	//Aggiungo alla lista dei nodi le sorgenti
	while (sources.count() != 0)
	{
		D3(PRINT(count << " "));
		nodes.insert(sources.get(0, true));
		count++;
	}

	//Aggiungo alla lista dei nodi i blocchi interni che sono curve o incroci.
	for (int i = 1; i < mapSize.y - 1; i++)
		for (int j = 1; j < mapSize.x-1; j++)
			if (!isEmptyBlock(map.getBlocco(i, j)->getTipo()) && !isRectBlock(map.getBlocco(i, j)->getTipo()))
			{
				D3(PRINT(count << " "));
				nodes.insert(Vector2i(j, i));
				count++;
			}

	GraphNode * currentPtr;
	TipoBlocco tipo;

	//Per ogni nodo, individuo e imposto dove possibile i collegamnti con i nodi vicini seguendo le quattro direzioni.
	for (int i = 0; i < nodes.count(); i++)
	{
		currentPtr = nodes.get(i);
		tipo = map.getBlocco(currentPtr->pos.y, currentPtr->pos.x)->getTipo();

		checkLinkSx(currentPtr, tipo, map);
		checkLinkDx(currentPtr, tipo, map);
		checkLinkUp(currentPtr, tipo, map);
		checkLinkDown(currentPtr, tipo, map);
	}
}


void Graph::findPath(Vector2i startPos, Vector2i_List &sources, Vector2i_List &path)
{
	//Se la posizione di partenza non esiste nella lista dei nodi
	if (nodes.getIndex(startPos) == -1 )
		return;

	const int count = nodes.count();

	//Creazione e inizializzazione dell'array delle distanze
	int * dist = new int[count];
	for (int i = 0; i < count; i++)
		dist[i] = 0;

	//Creazione e inizializzazione dell'array dei parent
	Vector2i * parent = new Vector2i[count];
	for (int i = 0; i < count; i++)
		parent[i] = nodes.get(i, false).pos;

	buildParentArray(parent, dist, count, startPos);

	D3(PRINT("\nStampo array dei parent"));
	D3(for (int i = 0; i < count; i++) PRINT(i<<" p: " <<parent[i].x <<", " <<parent[i].y <<"  d: " <<dist[i]));

	//Lista delle destinazioni raggiungibili
	Vector2i_List endSources;
	Vector2i endPos(0,0);

	//Indivuazione e inserimento delle destinazioni raggiungibili nella lista apposita.
	//Una nodo è considerato destinazione se è una sorgente e se il suo rispettivo valore 
	//nell'array delle distanze è diverso da 0.
	for (int i = 0; i < count; i++)
	{
		endPos = nodes.get(i)->pos;
		if (endPos != startPos && sources.get(endPos.x, endPos.y, false) != Vector2i(-1, -1) && dist[i] != 0)
			endSources.insertHead(endPos);
	}

	assert(endSources.count() > 0);
	//Estrazione casuale della destinazione dalla lista di destinazioni.
	endPos = endSources.get(rand() % endSources.count(), false);

	D2(PRINT("\n\nTrovo percorso fra " << startPos.x << ", " << startPos.y << " e " << endPos.x << ", " << endPos.y));

	buildPath(path, parent, count, endPos);

	delete [] dist;
	delete [] parent;
}


void Graph::addLink(GraphNode & currentNode, GraphNode & nextNode, Mappa &map, int numRect)
{
	Vector2i currentPos = currentNode.pos,
			 nextPos	= nextNode.pos,
			 mapSize	= map.getMapSize();


	if (currentPos.x == nextPos.x && currentPos.y == nextPos.y &&
		(currentPos.x < 0 || currentPos.x >= mapSize.x || currentPos.y < 0 || currentPos.y >= mapSize.y))
		return;

	//Si trova la posizione di adiacenza
	Direzione adjDir = Direzione::ND;

	if (currentPos.x == nextPos.x)
		if (currentPos.y < nextPos.y)
			adjDir = Direzione::GIU;
		else
			adjDir = Direzione::SU;

	else
		if (currentPos.y == nextPos.y)
		{
			if (currentPos.x < nextPos.x)
				adjDir = Direzione::DX;
			else
				adjDir = Direzione::SX;
		}

	int weight = getWeight(map.getBlocco(currentPos.y, currentPos.x)->getTipo()) + getWeight(map.getBlocco(nextPos.y, nextPos.x)->getTipo()) + numRect;
	
	//Si impostano le adiacenze
	currentNode.setAdiacenza(adjDir, &nextNode, weight);
	nextNode.setAdiacenza(getDirOpposta(adjDir), &currentNode, weight);
}

bool Graph::checkLinkable(TipoBlocco blocco, TipoBlocco curva1, TipoBlocco curva2, TipoBlocco cross3)
{
	if (blocco == curva1 ||	blocco == curva2 ||
		(isCross3Block(blocco) && blocco != cross3) ||
		blocco == TipoBlocco::CROSS4)
		return true;

	return false;
}

void Graph::checkLinkSx(GraphNode * currentPtr, TipoBlocco tipo, Mappa & map)
{
	if (checkLinkable(tipo, TipoBlocco::SX_TO_DOWN, TipoBlocco::SX_TO_UP, TipoBlocco::CROSS3_DX))
	{
		int counter = -1;
		GraphNode * nextPtr;
		TipoBlocco nextTipo;
		while (currentPtr->pos.x + counter >= 0)
		{
			nextPtr = nodes.get(currentPtr->pos.x + counter, currentPtr->pos.y);
			if (nextPtr != NULL)
			{
				nextTipo = map.getBlocco(nextPtr->pos.y, nextPtr->pos.x)->getTipo();
				if (checkLinkable(nextTipo, TipoBlocco::DX_TO_DOWN, TipoBlocco::DX_TO_UP, TipoBlocco::CROSS3_SX) || nextPtr->pos.x == 0)
				{
					addLink(*currentPtr, *nextPtr, map, std::abs(counter) - 1);
					return;
				}
			}

			counter--;
		}
	}
}

void Graph::checkLinkDx(GraphNode * currentPtr, TipoBlocco tipo, Mappa & map)
{
	if (checkLinkable(tipo, TipoBlocco::DX_TO_UP, TipoBlocco::DX_TO_DOWN, TipoBlocco::CROSS3_SX))
	{
		int counter = 1;
		GraphNode * nextPtr;
		TipoBlocco nextTipo;
		while (currentPtr->pos.x + counter < mapSize.x)
		{
			nextPtr = nodes.get(currentPtr->pos.x + counter, currentPtr->pos.y);
			if (nextPtr != NULL)
			{
				nextTipo = map.getBlocco(nextPtr->pos.y, nextPtr->pos.x)->getTipo();
				if (checkLinkable(nextTipo, TipoBlocco::SX_TO_UP, TipoBlocco::SX_TO_DOWN, TipoBlocco::CROSS3_DX) || nextPtr->pos.x == mapSize.x - 1)
				{
					addLink(*currentPtr, *nextPtr, map, counter - 1);
					return;
				}
			}

			counter++;
		}
	}
}

void Graph::checkLinkUp(GraphNode * currentPtr, TipoBlocco tipo, Mappa & map)
{
	if (checkLinkable(tipo, TipoBlocco::SX_TO_UP, TipoBlocco::DX_TO_UP, TipoBlocco::CROSS3_DOWN) )
	{
		int counter = -1;
		GraphNode * nextPtr;
		TipoBlocco nextTipo;
		while (currentPtr->pos.y + counter >= 0)
		{
			nextPtr = nodes.get(currentPtr->pos.x, currentPtr->pos.y + counter);
			if (nextPtr != NULL)
			{
				nextTipo = map.getBlocco(nextPtr->pos.y, nextPtr->pos.x)->getTipo();
				if (checkLinkable(nextTipo, TipoBlocco::SX_TO_DOWN, TipoBlocco::DX_TO_DOWN, TipoBlocco::CROSS3_UP) || nextPtr->pos.y == 0)
				{
					addLink(*currentPtr, *nextPtr, map, std::abs(counter) - 1);
					return;
				}
			}

			counter--;
		}
	}
}

void Graph::checkLinkDown(GraphNode * currentPtr, TipoBlocco tipo, Mappa & map)
{
	if (checkLinkable(tipo, TipoBlocco::SX_TO_DOWN, TipoBlocco::DX_TO_DOWN, TipoBlocco::CROSS3_UP))
	{
		int counter = 1;
		GraphNode * nextPtr;
		TipoBlocco nextTipo;
		while (currentPtr->pos.y + counter < mapSize.y)
		{
			nextPtr = nodes.get(currentPtr->pos.x, currentPtr->pos.y + counter);
			if (nextPtr != NULL)
			{
				nextTipo = map.getBlocco(nextPtr->pos.y, nextPtr->pos.x)->getTipo();
				if (checkLinkable(nextTipo, TipoBlocco::SX_TO_UP, TipoBlocco::DX_TO_UP, TipoBlocco::CROSS3_DOWN) || nextPtr->pos.y == mapSize.y - 1)
				{
					addLink(*currentPtr, *nextPtr, map, counter - 1);
					return;
				}
			}

			counter++;
		}
	}
}

int Graph::getWeight(TipoBlocco tipo)
{
	if (isRectBlock(tipo))
		return RECT_WEIGHT;

	if (isCurveBlock(tipo))
		return CURVE_WEIGHT;

	if (isCross3Block(tipo))
		return CROSS3_WEIGHT;

	if (isCross4Block(tipo))
		return CROSS4_WEIGHT;

	return 0;
}

void Graph::buildPath(Vector2i_List &path, Vector2i parent[], int count, Vector2i endPos)
{
	path.clean();

	Vector2i temp = endPos;
	int index = nodes.getIndex(temp);
	assert(index != -1 && index < count);

	//Si percorre all'indietro l'array dei parent partendo dalla posizione finale 
	//fino alla posizione iniziale, riconosciuta se ha parent uguale a se stessa.
	while (parent[index] != temp)
	{
		path.insertHead(temp);
		temp = parent[index];
		index = nodes.getIndex(temp);
		assert(index != -1 && index < count);
	}

	//Se si ha trovato un percorso, aggiungo la posizione iniziale alla lista.
	if (path.count() != 0)
	{
		assert(index != -1 && index < count);
		path.insertHead(temp);
	}
}

void Graph::buildParentArray(Vector2i parent[], int dist[], int count, Vector2i startPos)
{
	Vector2i_List stack;
	stack.insertHead(startPos);
	dist[nodes.getIndex(startPos)] = 0;

	GraphNode * currentNode = NULL;
	Vector2i currentPos;
	int currentIndex;
	while (stack.count() != 0)
	{
		currentPos = stack.get(0, true);
		currentIndex = nodes.getIndex(currentPos);
		currentNode = nodes.get(currentIndex);

		//Array di supporto per la visita delle adiacenze
		Vector2i adj[4];	//Posizione dei nodi adiacenti
		int w[4];			//Peso del collegamento dei nodi adiacenti			

		for (int i = 0; i < 4; i++)
		{
			//Inizializzazione degli array di supporto per la visita delle adiacenze
			adj[i] = Vector2i(-1, -1);
			w[i] = -1;

			int adjWeight = 0;
			GraphNode * adjPtr = currentNode->getAdiacenza(i, adjWeight);
			if (adjPtr != NULL)
			{
				Vector2i adjPos = adjPtr->pos;
				int adjIndex = nodes.getIndex(adjPos);
				
				assert(adjIndex >= 0 && adjIndex < count);

				//Se il nodo non è ancora stato visitato, cioè il parent del nodo è uguale al nodo stesso, e se il nodo non è il nodo iniziale..
				if ((parent[adjIndex].x == adjPos.x && parent[adjIndex].y == adjPos.y && (adjPos.x != startPos.x || adjPos.y != startPos.y)) || 
					//.. o se si trova una via più breve per raggiungere il nodo adiacente
					dist[adjIndex] > dist[currentIndex] + adjWeight)
				{
					//Se il nodo non è ancora stato visitato si aggiornano gli array
					if (parent[adjIndex].x == adjPos.x && parent[adjIndex].y == adjPos.y && (adjPos.x != startPos.x || adjPos.y != startPos.y))
					{
						w[i] = adjWeight;
						adj[i] = parent[adjIndex];
					}

					//Aggiorno le informazioni del nodo adiacente
					parent[adjIndex] = currentPos;
					dist[adjIndex] = dist[currentIndex] + adjWeight;
				}
			}
		}

		//Inserisco nello stack i nodi in ordine di peso decrescente
		//(quindi il prossimo ad essere estratto sarà il nodo con peso minore)
		insertStack(stack, adj, w);
	}
}

void Graph::insertStack(Vector2i_List & stack, Vector2i adj[], int weight[])
{
	for (int i = 0; i < 4; i++)
	{
		int j = findMaxAdj(weight);
		if (j != -1)
		{
			weight[j] = -1;
			stack.insertHead(adj[j]);
		}
	}
}

int Graph::findMaxAdj(int weight[])
{
	int max = weight[0];
	int imax = 0;
	for (int i = 1; i < 4; i++)
		if (max < weight[i])
		{
			max = weight[i];
			imax = i;
		}

	if (weight[imax] == -1)
		return -1;

	return imax;
}