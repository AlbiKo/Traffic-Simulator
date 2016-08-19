#include "../header/GraphNode_List.h"
#include "../header/Debug.h"

GraphNode_List::GraphNode_List()
{
	root = NULL;
	num_nodi = 0;
}

GraphNode_List::~GraphNode_List()
{
	D2(PRINT("Distruggo GraphNode list"));
	this->clean();
}

GraphNode_List::GraphNode_List(GraphNode r)
{
	root = new GraphNode_Node;
	root->node.copy(r);
	root->next = NULL;
	num_nodi = 1;
}

void GraphNode_List::insert(GraphNode r)
{
	GraphNode_Node *i = new GraphNode_Node;
	i->node.copy(r);
	i->next = NULL;
	if (root == NULL)
		root = i;
	else {
		GraphNode_Node *n = root;
		while (n->next != NULL)
			n = n->next;
		n->next = i;
	}
	num_nodi++;
}

GraphNode GraphNode_List::get(int index, bool del)
{
	if (index >= num_nodi) return GraphNode();
	GraphNode_Node *t = root;
	GraphNode_Node *previous = NULL;
	for (int i = 0; i < index; i++) {
		previous = t;
		t = t->next;
	}
	GraphNode temp = t->node;
	if (del) {
		if (t == root)
			root = t->next;
		else
			previous->next = t->next;
		delete t;
		num_nodi--;
		
	}

	return temp;
}

GraphNode * GraphNode_List::get(int index)
{
	if (index >= num_nodi) return NULL;
	GraphNode_Node *t = root;
	GraphNode_Node *previous = NULL;
	for (int i = 0; i < index; i++) {
		previous = t;
		t = t->next;
	}

	return &(t->node);
}

GraphNode GraphNode_List::get(int x, int y, bool del)
{
	GraphNode_Node *t = root;
	GraphNode_Node *previous = NULL;

	if (t == NULL) return GraphNode();

	while (t != NULL && (t->node.getPos().x != x || t->node.getPos().y != y))
	{
		previous = t;
		t = t->next;
	}

	if (t != NULL)
	{
		GraphNode temp = t->node;
		if (del)
		{
			if (t == root)
				root = t->next;
			else
				previous->next = t->next;

			delete t;
			num_nodi--;
		}
		return temp;
	}

	return GraphNode();
}

GraphNode * GraphNode_List::get(int x, int y)
{
	GraphNode_Node *t = root;
	GraphNode_Node *previous = NULL;

	if (t == NULL) return NULL;

	while (t != NULL && (t->node.getPos().x != x || t->node.getPos().y != y))
	{
		previous = t;
		t = t->next;
	}

	if (t != NULL)
		return &(t->node);

	return NULL;
}

int GraphNode_List::getIndex(Vector2i pos)
{
	GraphNode_Node *t = root;
	GraphNode_Node *previous = NULL;

	int i = 0;

	if (t == NULL) return -1;

	while (t != NULL && (t->node.getPos().x != pos.x || t->node.getPos().y != pos.y))
	{
		previous = t;
		t = t->next;
		i++;
	}

	if (t != NULL)
		return i;

	return -1;
}

void GraphNode_List::clean()
{
	while (num_nodi != 0)
		get(0, true);
}
