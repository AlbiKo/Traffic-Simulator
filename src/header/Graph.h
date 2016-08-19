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
	Vector2i_List findPath(Vector2i startPos, Vector2i endPos);
private:
	GraphNode_List nodes;
	Vector2i mapSize;

	const int	RECT_WEIGHT		= 1,
				CURVE_WEIGHT	= 2,
				CROSS3_WEIGHT	= 3,
				CROSS4_WEIGHT	= 4;

	void addLink(GraphNode &currentNode, GraphNode &nextNode,  Mappa &map, int numRect);

	bool checkLinkable(TipoBlocco blocco, TipoBlocco curva1, TipoBlocco curva2, TipoBlocco cross3);
	void checkLinkSx(GraphNode * currentPtr, TipoBlocco tipo, Mappa &map);
	void checkLinkDx(GraphNode * currentPtr, TipoBlocco tipo, Mappa &map);
	void checkLinkUp(GraphNode * currentPtr, TipoBlocco tipo, Mappa &map);
	void checkLinkDown(GraphNode * currentPtr, TipoBlocco tipo, Mappa &map);

	int getPeso(TipoBlocco tipo);
};


#endif // !GRAPH_INCLUDE

