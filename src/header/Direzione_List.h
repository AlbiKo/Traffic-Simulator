#ifndef DIREZIONE_LIST_INCLUDE
#define DIREZIONE_LIST_INCLUDE

#include "Direzione.h"

struct Direzione_Node {
	Direzione node;
	Direzione_Node * next;
};

class Direzione_List {

private:
	Direzione_Node *root;
	int num_nodi;

public:
	Direzione_List();
	~Direzione_List();
	Direzione_List(Direzione r);
	
	inline int count()
	{
		return num_nodi;
	}

	void insert(Direzione r);
	Direzione get(int index, bool del);
	Direzione get(Direzione d, bool del);
	void clean();

};

#endif