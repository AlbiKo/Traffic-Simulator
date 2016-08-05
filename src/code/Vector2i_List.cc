#include "../header/Vector2i_List.h"
#include "../header/Debug.h"

using namespace sf;

Vector2i_List::Vector2i_List()
{
	root = NULL;
	num_nodi = 0;
}

Vector2i_List::Vector2i_List(sf::Vector2i r)
{
	root = new Vector2i_Node;
	root->node = r;
	root->next = NULL;
	num_nodi = 1;
}

void Vector2i_List::insert(sf::Vector2i r)
{
	Vector2i_Node *i = new Vector2i_Node;
	i->node = r;
	i->next = NULL;
	if (root == NULL)
		root = i;
	else {
		Vector2i_Node *n = root;
		while (n->next != NULL)
			n = n->next;
		n->next = i;
	}
	num_nodi++;
}

Vector2i Vector2i_List::get(int index, bool del)
{
	if (index >= num_nodi) return Vector2i(-1, -1);
	Vector2i_Node *t = root;
	Vector2i_Node *previous = NULL;
	for (int i = 0; i < index; i++) {
		previous = t;
		t = t->next;
	}
	Vector2i temp = t->node;
	if (del) {
		if (t == root)
			root = t->next;
		else
			previous->next = t->next;
		delete t;
		num_nodi--;
		
	}
	D1(PRINT(num_nodi));
	return temp;
}

Vector2i Vector2i_List::get(int x, int y, bool del)
{
	Vector2i_Node *t = root;
	Vector2i_Node *previous = NULL;

	if (t == NULL) return Vector2i(-1, -1);

	while (t != NULL && (t->node.x != x && t->node.y != y))
	{
		previous = t;
		t = t->next;
	}

	if (t != NULL)
	{
		Vector2i temp = t->node;
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

	return Vector2i(-1, -1);
}

void Vector2i_List::clean()
{
	while (num_nodi != 0)
		get(0, true);
}
