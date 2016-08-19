#include "../header/Macchina_List.h"
#include "../header/Debug.h"



Macchina_List::Macchina_List()
{
	root = NULL;
	num_nodi = 0;
}

Macchina_List::~Macchina_List()
{
	this->clean();
}


void Macchina_List::add(Vector2i pos, Direzione dir)
{
	Macchina_Node *t = new Macchina_Node;
	t->node.setPosition(pos);
	t->node.changeDirection(dir);

	if (root == NULL)
		t->next = NULL;
	else 
		t->next = root;
	root = t;
	num_nodi++;
}

void Macchina_List::add(Vector2i pos, Direzione dir, Color color)
{
	add(pos, dir);
	root->node.setColor(color);
}

void Macchina_List::insert(Macchina i)
{
	Macchina_Node *t = new Macchina_Node;
	t->node.copy(i);

	if (root == NULL)
		t->next = NULL;
	else
		t->next = root;
	root = t;
	num_nodi++;
}

Macchina * Macchina_List::get(int index)
{
	if (index >= num_nodi) return NULL; 
	Macchina_Node *t = root;
	for (int i = 0; i < index; i++)
		t = t->next;
	return &t->node;
}


void Macchina_List::clean()
{
	while (root != NULL) {
		Macchina_Node *t = root->next;
		delete root;
		root = t;
	}
	num_nodi = 0;
}

bool Macchina_List::remove(int index)
{
	if (index >= num_nodi) return NULL;
	Macchina_Node *t = root;
	Macchina_Node *previous = NULL;
	for (int i = 0; i < index; i++) {
		previous = t;
		t = t->next;
	}
	previous->next = t->next;
	delete t;
	return true;
}
