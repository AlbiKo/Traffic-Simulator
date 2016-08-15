#ifndef BLOCCO_LIST_INCLUDE
#define BLOCCO_LIST_INCLUDE

#include "Blocco.h"

struct Blocco_Node {
	Blocco node;
	Blocco_Node * next;
};

class Blocco_List {

private:
	Blocco_Node *root;
	int num_nodi;

public:
	Blocco_List();
	~Blocco_List();
	Blocco_List(Blocco r);
	
	inline int count()
	{
		return num_nodi;
	}

	void insert(Blocco r);
	Blocco get(int index, bool del);
	Blocco get(int x, int y, bool del);
	void clean();

};

#endif