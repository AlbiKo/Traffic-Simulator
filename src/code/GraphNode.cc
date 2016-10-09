#include "../header/GraphNode.h"
#include "../header/Debug.h"


GraphNode::GraphNode()
{
	initNodo();
}

GraphNode::GraphNode(Vector2i startPos)
{
    initNodo();
	pos = startPos;
}

GraphNode::~GraphNode()
{
}

void GraphNode::initNodo()
{
	pos = Vector2i(-1, -1);

	for (int i = 0; i < 4; i++)
	{
		adiacenze[i].nodo = NULL;
		adiacenze[i].peso = 0;
	}
}

void GraphNode::copy(const GraphNode &node)
{
	pos = node.pos;

	for (int i = 0; i < 4; i++)
		adiacenze[i] = node.adiacenze[i];
}

bool GraphNode::setAdiacenza(Direzione dir, GraphNode * nodo, int peso)
{
	switch (dir)
	{
	case Direzione::SU:
		return setAdiacenza(UP_INDEX, nodo, peso);
	case Direzione::GIU:
		return setAdiacenza(DOWN_INDEX, nodo, peso);
	case Direzione::DX:
		return setAdiacenza(RIGHT_INDEX, nodo, peso);
	case Direzione::SX:
		return setAdiacenza(LEFT_INDEX, nodo, peso);
	default:
		return false;
	}
}

bool GraphNode::setAdiacenza(int i, GraphNode * nodo, int peso)
{
	if (i < 0 || i > 3)
		return NULL;
	
	if (i >= 0 && i <= 3 && adiacenze[i].nodo == NULL)
	{
		adiacenze[i].nodo = nodo;

		if (nodo == NULL)
			adiacenze[i].peso = 0;
		else
			adiacenze[i].peso = peso;

		return true;
	}
	return false;
}

GraphNode * GraphNode::getAdiacenza(int i, int &peso)
{
	if (i < 0 || i > 3)
		return NULL;

	peso = adiacenze[i].peso;
	return adiacenze[i].nodo;
}

GraphNode * GraphNode::getAdiacenza(Direzione dir, int &peso)
{
	switch (dir)
	{
	case Direzione::SU:
		return getAdiacenza(UP_INDEX, peso);
	case Direzione::GIU:
		return getAdiacenza(DOWN_INDEX, peso);
	case Direzione::DX:
		return getAdiacenza(RIGHT_INDEX, peso);
	case Direzione::SX:
		return getAdiacenza(LEFT_INDEX, peso);
	default:
		return NULL;
	}
}
