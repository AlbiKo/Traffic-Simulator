#include "../header/Blocco_List.h"
#include "../header/Debug.h"

Blocco_List::Blocco_List()
{
	root = NULL;
	num_nodi = 0;
}

Blocco_List::~Blocco_List()
{
	D2(PRINT("Distruggo vector list"));
	this->clean();
}

Blocco_List::Blocco_List(Blocco r)
{
	root = new Blocco_Node;
	root->node = r;
	root->next = NULL;
	num_nodi = 1;
}

void Blocco_List::insert(Blocco r)
{
	Blocco_Node *i = new Blocco_Node;
	i->node = r;
	i->next = NULL;
	if (root == NULL)
		root = i;
	else {
		Blocco_Node *n = root;
		while (n->next != NULL)
			n = n->next;
		n->next = i;
	}
	num_nodi++;
}

Blocco Blocco_List::get(int index, bool del)
{
	if (index >= num_nodi) return Blocco();
	Blocco_Node *t = root;
	Blocco_Node *previous = NULL;
	for (int i = 0; i < index; i++) {
		previous = t;
		t = t->next;
	}
	Blocco temp = t->node;
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

Blocco Blocco_List::get(int x, int y, bool del)
{
	Blocco_Node *t = root;
	Blocco_Node *previous = NULL;

	if (t == NULL) return Blocco();

	while (t != NULL && (t->node.coordBlocco().x != x || t->node.coordBlocco().y != y))
	{
		previous = t;
		t = t->next;
	}

	if (t != NULL)
	{
		Blocco temp = t->node;
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

	return Blocco();
}

void Blocco_List::clean()
{
	while (num_nodi != 0)
		get(0, true);
}
