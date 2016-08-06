#include "../header/Direzione_List.h"
#include "../header/Debug.h"

Direzione_List::Direzione_List()
{
	root = NULL;
	num_nodi = 0;
}

Direzione_List::~Direzione_List()
{
	this->clean();
}

Direzione_List::Direzione_List(Direzione r)
{
	root = new Direzione_Node;
	root->node = r;
	root->next = NULL;
	num_nodi = 1;
}

void Direzione_List::insert(Direzione r)
{
	Direzione_Node *i = new Direzione_Node;
	i->node = r;
	i->next = NULL;
	if (root == NULL)
		root = i;
	else {
		Direzione_Node *n = root;
		while (n->next != NULL)
			n = n->next;
		n->next = i;
	}
	num_nodi++;
}

Direzione Direzione_List::get(int index, bool del)
{
	if (index >= num_nodi)
		return Direzione::ND;

	Direzione_Node *t = root;
	Direzione_Node *previous = NULL;
	for (int i = 0; i < index; i++) {
		previous = t;
		t = t->next;
	}
	Direzione temp = t->node;
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

Direzione Direzione_List::get(Direzione d, bool del)
{
	Direzione_Node *t = root;
	Direzione_Node *previous = NULL;

	if (t == NULL) return Direzione::ND;

	while (t != NULL && (t->node != d))
	{
		previous = t;
		t = t->next;
	}

	if (t != NULL)
	{
		Direzione temp = t->node;
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

	return Direzione::ND;
}

void Direzione_List::clean()
{
	while (num_nodi != 0)
		get(0, true);
}
