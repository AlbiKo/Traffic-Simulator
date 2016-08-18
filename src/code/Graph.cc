#include "../header/Graph.h"
#include "../header/Debug.h"
Graph::Graph()
{
	//nodes = GraphNode_List();
}

Graph::~Graph()
{
	nodes.clean();
}

void Graph::buildGraph(Mappa &m)
{
	Vector2i mapSize = m.getMapSize();
	
	for (int i = 0; i < mapSize.y; i++)
		for (int j = 0; j < mapSize.x; j++)
			if (!isEmptyBlock(m.getBlocco(i, j)->getTipo()))
				nodes.insert(Vector2i(j, i));

	GraphNode * currentPtr,  * nextPtr;
	TipoBlocco tipo;
	for (int i = 0; i < nodes.count(); i++)
	{
		currentPtr = nodes.get(i);
		tipo = m.getBlocco(currentPtr->getPos().y, currentPtr->getPos().x)->getTipo();

		nextPtr = nodes.get(currentPtr->getPos().x, currentPtr->getPos().y + 1);
		if (nextPtr != NULL)
			if (checkLinkable(tipo, TipoBlocco::VERTICAL, TipoBlocco::SX_TO_DOWN, TipoBlocco::DX_TO_DOWN, TipoBlocco::CROSS3_UP))
				addLink(*currentPtr, *nextPtr, m);


		nextPtr = nodes.get(currentPtr->getPos().x, currentPtr->getPos().y - 1);
		if (nextPtr != NULL)
			if (checkLinkable(tipo, TipoBlocco::VERTICAL, TipoBlocco::SX_TO_UP, TipoBlocco::DX_TO_UP, TipoBlocco::CROSS3_DOWN))
				addLink(*currentPtr, *nextPtr, m);

		nextPtr = nodes.get(currentPtr->getPos().x + 1, currentPtr->getPos().y);
		if (nextPtr != NULL)
			if (checkLinkable(tipo, TipoBlocco::HORIZONTAL, TipoBlocco::DX_TO_UP, TipoBlocco::DX_TO_DOWN, TipoBlocco::CROSS3_SX))
				addLink(*currentPtr, *nextPtr, m);

		nextPtr = nodes.get(currentPtr->getPos().x - 1, currentPtr->getPos().y);
		if (nextPtr != NULL)
			if (checkLinkable(tipo, TipoBlocco::HORIZONTAL, TipoBlocco::SX_TO_DOWN, TipoBlocco::SX_TO_UP, TipoBlocco::CROSS3_DX))
				addLink(*currentPtr, *nextPtr, m);
	}

}

void Graph::addLink(GraphNode & currentNode, GraphNode & nextNode, Mappa &m)
{
	Vector2i currentPos = currentNode.getPos(),
			 nextPos	= nextNode.getPos(),
			 mapSize	= m.getMapSize();


	if (currentPos.x == nextPos.x && currentPos.y == nextPos.y &&
		(currentPos.x < 0 || currentPos.x >= mapSize.x || currentPos.y < 0 || currentPos.y >= mapSize.y))
		return;

	int peso = getPeso(m.getBlocco(currentPos.y, currentPos.x)->getTipo());
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
	if (adjDir != Direzione::ND)
		std::cerr << currentPos.x << ", " << currentPos.y << " <-> " << nextPos.x << ", " << nextPos.y << std::endl;

	currentNode.setAdiacenza(adjDir, &nextNode, peso);
	nextNode.setAdiacenza(getDirOpposta(adjDir), &currentNode, peso);
}

bool Graph::checkLinkable(TipoBlocco blocco, TipoBlocco rect, TipoBlocco curva1, TipoBlocco curva2, TipoBlocco cross3)
{
	if (blocco == rect ||
		blocco == curva1 ||	blocco == curva2 ||
		(isCross3Block(blocco) && blocco != cross3) ||
		blocco == TipoBlocco::CROSS4)
		return true;

	return false;
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
