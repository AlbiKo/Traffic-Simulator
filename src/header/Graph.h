#ifndef GRAPH_INCLUDE
#define GRAPH_INCLUDE

#include "GraphNode.h"
#include "GraphNode_List.h"
#include "Mappa.h"

class Graph
{
public:
	Graph();
	~Graph();

	void buildGraph(Mappa &m);
	Vector2i_List findPath();
private:
	GraphNode_List nodes;

	const int	RECT_WEIGHT		= 1,
				CURVE_WEIGHT	= 2,
				CROSS3_WEIGHT	= 3,
				CROSS4_WEIGHT	= 4;

	void addLink(GraphNode &currentNode, GraphNode &nextNode,  Mappa &m);

	bool checkLinkable(TipoBlocco blocco, TipoBlocco rect, TipoBlocco curva1, TipoBlocco curva2, TipoBlocco cross3);
	int getPeso(TipoBlocco tipo);
};


#endif // !GRAPH_INCLUDE

