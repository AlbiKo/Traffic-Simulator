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

int max(int v[], int n, int val)
{
	int max = v[0];
	int imax = 0;
	for (int i = 1; i < n; i++)
		if (max < v[i])
		{
			max = v[i];
			imax = i;
		}

	if (v[imax] == val)
		return -1;

	v[imax] = val;
	return imax;
}


Vector2i_List Graph::findPath(Vector2i startPos, Vector2i endPos)
{
	if (nodes.getIndex(startPos) == -1 || nodes.getIndex(endPos) == -1)
		return Vector2i_List();

	D1(PRINT("\n\nTrovo percorso fra " <<startPos.x <<", " <<startPos.y <<" e " <<endPos.x <<", " <<endPos.y));

	Vector2i_List percorso, stack;

	const int count = nodes.count();

	int * dist = new int[count];
	for (int i = 0; i < count; i++)
		dist[i] = 0;

	Vector2i * parent = new Vector2i[count];
	for (int i = 0; i < count; i++)
		parent[i] = nodes.get(i, false).getPos();

	stack.insertHead(startPos);
	dist[nodes.getIndex(startPos)] = 0;

	GraphNode * tempNode = NULL;
	Vector2i temp;
	int index;
	while (stack.count() != 0)
	{
		temp = stack.get(0, true);
		index = nodes.getIndex(temp);
		assert(index >= 0 && index < count);
		tempNode = nodes.get(index);
		
		Vector2i adj[4];
		int w[4];
		for (int i = 0; i < 4; i++)
		{
			adj[i] = Vector2i(-1, -1);
			w[i] = -1;
			int peso = 0;
			GraphNode * adjPtr = tempNode->getAdiacenza(i, peso);
			if (adjPtr != NULL)
			{
				Vector2i adjPos = adjPtr->getPos();
				int tempIndex = nodes.getIndex(adjPos);
				assert(tempIndex >= 0 && tempIndex < count);
				Vector2i pos = parent[tempIndex];
				
				if ((pos.x == adjPos.x && pos.y == adjPos.y) || dist[tempIndex] > dist[index] + peso)
				{
					if (pos.x == adjPos.x && pos.y == adjPos.y)//Controllo se è stata visitata
					{
						w[i] = peso;
						adj[i] = pos;
					}
					parent[tempIndex] = temp;
					dist[tempIndex] = dist[index] + peso;
				}
			}
		}
		
		for (int i = 0; i < 4; i++)
		{
			int j = max(w, 4, -1);
			if (j != -1)
				stack.insertHead(adj[j]);
		}
	}

	//Matrice di struttura per parent e dist
	//Lista dei nodi già visitati (potrei fare i non visitati abbiano se stessi come parent)
	//Decidere chi viene scelto in caso di peso uguale
	//Usare uno stack per la visita di percorsi alternativi (serve inserimento in testa in vector list

	//Mi prendo dalla lista la startPos
	//Aggiungo ai già visitati e aggiorno la matrice
	//Prendo l'adiacenza con peso minore (se uguale ci si deve basare sul criterio scelto)
	//Aggiungo ai già visitati e aggiorno la matrice se non già visitato ho dist minore

	D1(PRINT("\nStampo array"));
	for (int i = 0; i < count; i++)
	{
		D1(PRINT(i<<" p: " <<parent[i].x <<", " <<parent[i].y <<"  d: " <<dist[i]));
	}

	delete [] dist;
	delete [] parent;
	return percorso;
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

	int peso = getPeso(map.getBlocco(currentPos.y, currentPos.x)->getTipo()) + getPeso(map.getBlocco(nextPos.y, nextPos.x)->getTipo()) + numRect - 1;
	
	if (adjDir != Direzione::ND)
		std::cerr << currentPos.x << ", " << currentPos.y << " <--> " << nextPos.x << ", " << nextPos.y <<" peso: "<<peso<< std::endl;

	//Si mettono le adiacenze
	currentNode.setAdiacenza(adjDir, &nextNode, peso);
	nextNode.setAdiacenza(getDirOpposta(adjDir), &currentNode, peso);
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

int Graph::getPeso(TipoBlocco tipo)
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
