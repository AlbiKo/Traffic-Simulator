#include "../header/GraphNode.h"
#include "../header/Debug.h"


GraphNode::GraphNode()
{
	initNodo();

	D3(PRINT("Nodo base creato"));
}

GraphNode::GraphNode(Vector2i pos)
{
    initNodo();
	this->pos = pos;

	D1(PRINT("Nodo creato: posizione " << pos.x << ", " << pos.y));
}

GraphNode::~GraphNode()
{
}

void GraphNode::copy(GraphNode gn)
{
	pos = gn.pos;

	for (int i = 0; i < 4; i++)
		adiacenze[i] = gn.adiacenze[i];
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

void GraphNode::setPos(Vector2i pos)
{
	this->pos = pos;
}

bool GraphNode::setAdiacenza(Direzione dir, GraphNode * nodo, int peso)
{
	D1(PRINT("Imposto adiacenza di " << pos.x << ", " << pos.y <<" direzione " <<toString(dir)));
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
	assert(i >= 0 && i <= 3);
	D1(PRINT("Nodo: scrivo adiacenza in " <<i <<": " <<nodo));

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

bool GraphNode::setAdiacenza(GraphNode * nodo)
{
	int i;

	//Si cerca la prima posizione libera
	for (i = 0; i < 4 && adiacenze[i].nodo != NULL; i++);

	if (i == 4)
	{
		D2(PRINT("Nodo: adiacenza "<<nodo <<" non impostata; nessuna posizione libera"));
		return false;
	}
	else
	{
		setAdiacenza(i, nodo, 0);
		D1(PRINT("Nodo adiacenza "<<nodo <<"impostata alla posizione " <<i));
		return true;
	}
}

GraphNode * GraphNode::getAdiacenza(int i, int &peso)
{
	assert(i >= 0 && i <= 3);
	D1(PRINT("Nodo: leggo adiacenza in " << i));

	if (i < 0 || i > 3)
	{
		D1(PRINT("Nodo: letta adiacenza " << adiacenze[i].nodo << " di peso " << adiacenze[i].peso));
		adiacenze[i].peso = 0;
		return NULL;
	}

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
