#include "../header/GraphNode.h"
#include "../header/Debug.h"


GraphNode::GraphNode()
{
	initNodo();

	D3(PRINT("Nodo base creato"));
}

GraphNode::GraphNode(Vector2i startPos)
{
    initNodo();
	pos = startPos;

	D1(PRINT("Nodo creato: posizione " << pos.x << ", " << pos.y));
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
	D1(PRINT("\nImposto adiacenza di " << pos.x << ", " << pos.y <<" direzione " <<toString(dir)));
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
	
	D1( if (nodo != NULL) PRINT("Nodo: scrivo adiacenza in dir " <<i <<": " <<nodo->pos.x <<", " <<nodo->pos.y));

	if (i >= 0 && i <= 3 && adiacenze[i].nodo == NULL)
	{
		adiacenze[i].nodo = nodo;

		if (nodo == NULL)
			adiacenze[i].peso = 0;
		else
			adiacenze[i].peso = peso;

		D1(PRINT("Peso: " <<peso));

		return true;
	}
	D1(PRINT("Adiacenza non inserita (già presente o fuori range)"));
	return false;
}

GraphNode * GraphNode::getAdiacenza(int i, int &peso)
{
	if (i < 0 || i > 3)
		return NULL;

	D2(PRINT("Nodo: letta adiacenza " << adiacenze[i].nodo << " di peso " << adiacenze[i].peso));
	
	peso = adiacenze[i].peso;
	return adiacenze[i].nodo;
}

GraphNode * GraphNode::getAdiacenza(Direzione dir, int &peso)
{
	D1(PRINT("Ottengo adiacenza di " << pos.x << ", " <<pos.y << " direzione " << toString(dir)));
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
