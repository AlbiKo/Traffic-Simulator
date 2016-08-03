#ifndef VECTOR2I_LIST_INCLUDE
#define VECTOR2I_LIST_INCLUDE

#include <SFML/Graphics.hpp>

using namespace sf;

struct Vector2i_Node {
	Vector2i node;
	Vector2i_Node * next;
};

class Vector2i_List {

private:
	Vector2i_Node *root;
	int num_nodi;

public:
	Vector2i_List();
	Vector2i_List(Vector2i r);
	void insert(Vector2i r);
	Vector2i get(int index, bool del);
	Vector2i get(int x, int y, bool del);
	void clean();

};

#endif