#include "../header/MacchinaPtr_List.h"
#include "../header/Debug.h"


MacchinaPtr_List::MacchinaPtr_List()
{
	root = NULL;
	num_nodi = 0;
}

MacchinaPtr_List::~MacchinaPtr_List()
{
	this->clean();
}

void MacchinaPtr_List::insert(Macchina * i)
{
	
	MacchinaPtr_Node *t = new MacchinaPtr_Node;
	t->node = i;
	t->next = NULL;
	
	if (root == NULL)
		root = t;
	else {
		MacchinaPtr_Node *n = root, * prev;
		while (n != NULL && n->node != i)
		{
			prev = n;
			n = n->next;
		}
		if (n == NULL)
			prev->next = t;
		else
		{
			delete t;
			return;
		}
	}

	num_nodi++;
}

Macchina * MacchinaPtr_List::get(int index)
{
	if (index >= num_nodi) return NULL; 
	MacchinaPtr_Node *t = root;
	for (int i = 0; i < index; i++)
		t = t->next;
	return t->node;
}


void MacchinaPtr_List::clean()
{
	while (root != NULL) {
		MacchinaPtr_Node *t = root->next;
		delete root;
		root = t;
	}
	num_nodi = 0;
}

bool MacchinaPtr_List::remove(int index)
{
	std::cerr << "Rimuovo \n";
	if (index >= num_nodi) return NULL;
	MacchinaPtr_Node *t = root;
	MacchinaPtr_Node *previous = NULL;
	for (int i = 0; i < index; i++) {
		previous = t;
		t = t->next;
	}
	num_nodi--;
	previous->next = t->next;
	delete t;
	return true;
}


bool MacchinaPtr_List::remove(Macchina * m)
{
	
	MacchinaPtr_Node *t = root;
	MacchinaPtr_Node *previous = NULL;
	
	assert(m != NULL && 34);

	while (t != NULL && t->node != m)
	{
		previous = t;
		t = t->next;
	}

	if (t != NULL)
	{
		std::cerr << "Rimuovo " << m << " \n";
		if (t == root)
			root = root->next;
		
		else
			previous->next = t->next;

		delete t;
		num_nodi--;
		return true;
	}
	
	return false;
}