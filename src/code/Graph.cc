#include "../header/Graph.h"
#include "../header/Debug.h"
Graph::Graph()
{
	//nodes = GraphNode_List();
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
	
	int count = 0; //PER DEBUG
	Vector2i_List sorg;
	map.getSorgenti(sorg);
	
	while (sorg.count() != 0)
	{
		std::cout << count << " ";
		nodes.insert(sorg.get(0, true));
		count++;
	}
	for (int i = 1; i < mapSize.y - 1; i++)
		for (int j = 1; j < mapSize.x-1; j++)
			if (!isEmptyBlock(map.getBlocco(i, j)->getTipo()) && !isRectBlock(map.getBlocco(i, j)->getTipo()))
			{
				std::cout << count << " ";
				nodes.insert(Vector2i(j, i));
				count++;
			}

	GraphNode * currentPtr;
	TipoBlocco tipo;

	for (int i = 0; i < nodes.count(); i++)
	{
		currentPtr = nodes.get(i);
		tipo = map.getBlocco(currentPtr->getPos().y, currentPtr->getPos().x)->getTipo();

		checkLinkSx(currentPtr, tipo, map);
		checkLinkDx(currentPtr, tipo, map);
		checkLinkUp(currentPtr, tipo, map);
		checkLinkDown(currentPtr, tipo, map);
	
	}

}



void Graph::findPath(Vector2i startPos, Vector2i endPos, Vector2i_List &path)
{
	if (nodes.getIndex(startPos) == -1 || nodes.getIndex(endPos) == -1)
		return;

	D1(PRINT("\n\nTrovo percorso fra " <<startPos.x <<", " <<startPos.y <<" e " <<endPos.x <<", " <<endPos.y));
	std::cerr << "Destinazione " << endPos.x << ", " << endPos.y << " \n";
	const int count = nodes.count();

	int * dist = new int[count];
	for (int i = 0; i < count; i++)
		dist[i] = 0;

	Vector2i * parent = new Vector2i[count];
	for (int i = 0; i < count; i++)
		parent[i] = nodes.get(i, false).getPos();

	buildParentArray(parent, dist, count, startPos);

	D1(PRINT("\nStampo array"));
	D1(for (int i = 0; i < count; i++) PRINT(i<<" p: " <<parent[i].x <<", " <<parent[i].y <<"  d: " <<dist[i]));

	buildPath(path, parent, count, endPos);
	delete [] dist;
	delete [] parent;
}


void Graph::addLink(GraphNode & currentNode, GraphNode & nextNode, Mappa &map, int numRect)
{
	Vector2i currentPos = currentNode.getPos(),
			 nextPos	= nextNode.getPos(),
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

	//Si trova il peso
	if (numRect < 0)
		numRect *= -1;

	int weight = getWeight(map.getBlocco(currentPos.y, currentPos.x)->getTipo()) + getWeight(map.getBlocco(nextPos.y, nextPos.x)->getTipo()) + numRect - 1;
	
	//Si mettono le adiacenze
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
		while (currentPtr->getPos().x + counter >= 0)
		{
			nextPtr = nodes.get(currentPtr->getPos().x + counter, currentPtr->getPos().y);
			if (nextPtr != NULL)
			{
				nextTipo = map.getBlocco(nextPtr->getPos().y, nextPtr->getPos().x)->getTipo();
				if (checkLinkable(nextTipo, TipoBlocco::DX_TO_DOWN, TipoBlocco::DX_TO_UP, TipoBlocco::CROSS3_SX) || nextPtr->getPos().x == 0)
				{
					addLink(*currentPtr, *nextPtr, map, counter);
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
		while (currentPtr->getPos().x + counter < mapSize.x)
		{
			nextPtr = nodes.get(currentPtr->getPos().x + counter, currentPtr->getPos().y);
			if (nextPtr != NULL)
			{
				nextTipo = map.getBlocco(nextPtr->getPos().y, nextPtr->getPos().x)->getTipo();
				if (checkLinkable(nextTipo, TipoBlocco::SX_TO_UP, TipoBlocco::SX_TO_DOWN, TipoBlocco::CROSS3_DX) || nextPtr->getPos().x == mapSize.x - 1)
				{
					addLink(*currentPtr, *nextPtr, map, counter);
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
		while (currentPtr->getPos().y + counter >= 0)
		{
			nextPtr = nodes.get(currentPtr->getPos().x, currentPtr->getPos().y + counter);
			if (nextPtr != NULL)
			{
				nextTipo = map.getBlocco(nextPtr->getPos().y, nextPtr->getPos().x)->getTipo();
				if (checkLinkable(nextTipo, TipoBlocco::SX_TO_DOWN, TipoBlocco::DX_TO_DOWN, TipoBlocco::CROSS3_UP) || nextPtr->getPos().y == 0)
				{
					addLink(*currentPtr, *nextPtr, map, counter);
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
		while (currentPtr->getPos().y + counter < mapSize.y)
		{
			nextPtr = nodes.get(currentPtr->getPos().x, currentPtr->getPos().y + counter);
			if (nextPtr != NULL)
			{
				nextTipo = map.getBlocco(nextPtr->getPos().y, nextPtr->getPos().x)->getTipo();
				if (checkLinkable(nextTipo, TipoBlocco::SX_TO_UP, TipoBlocco::DX_TO_UP, TipoBlocco::CROSS3_DOWN) || nextPtr->getPos().y == mapSize.y - 1)
				{
					addLink(*currentPtr, *nextPtr, map, counter);
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
	D1(PRINT("Costruzione percorso: " <<count));

	Vector2i temp = endPos;
	int index = nodes.getIndex(temp);
	assert(index != -1 && index < count);
	while (parent[index] != temp)
	{
		D1(PRINT(temp.x <<" " <<temp.y <<" index " <<index));
		path.insertHead(temp);
		temp = parent[index];
		index = nodes.getIndex(temp);
		assert(index != -1 && index < count);
	}

	if (path.count() != 0)
	{
		assert(index != -1 && index < count);
		D1(PRINT(temp.x << " " << temp.y<< " index " << index));
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

		D3(PRINT("\nNodo attaule " << currentPos.x << ", " << currentPos.y << " di indice " << currentIndex << " tempNode " << currentNode->getPos().x << ", " << currentNode->getPos().y));

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
				Vector2i adjPos = adjPtr->getPos();
				int adjIndex = nodes.getIndex(adjPos);
				
				assert(adjIndex >= 0 && adjIndex < count);
				D3(PRINT("adjPos " << adjPos.x << ", " << adjPos.y << " di indice " << adjIndex << " e peso " << adjWeight << " pos " << parent[adjIndex].x << ", " << parent[adjIndex].y));

				//Se il nodo non è ancora stato visitato, cioè il parent del nodo è uguale al nodo stesso, e se il nodo non è il nodo iniziale..
				if ((parent[adjIndex].x == adjPos.x && parent[adjIndex].y == adjPos.y && (adjPos.x != startPos.x || adjPos.y != startPos.y)) || 
					//.. o se si trova una via più breve per raggiungere il nodo adiacente
					dist[adjIndex] > dist[currentIndex] + adjWeight)
				{
					//Se il nodo non è ancora stato visitato si aggiornano gli array
					if (parent[adjIndex].x == adjPos.x && parent[adjIndex].y == adjPos.y && (adjPos.x != startPos.x || adjPos.y != startPos.y))
					{
						D3(PRINT("Non visitato"));
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
			stack.insertHead(adj[j]);
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

	weight[imax] = -1;
	return imax;
}
