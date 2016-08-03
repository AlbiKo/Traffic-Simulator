#include "../header/Vector2i_List.h"

using namespace Vector2i_List;

Vector2i_List() {
  root = NULL;
  num_nodi=0;
}

Vector2i_List(sf::Vector2i r) {
  root = new Nodo();
  root->node = r;
  root->next = NULL;
  num_nodi=1;
}

void insert(sf::Vector2i r) {
  Nodo *i = new Nodo();
  i->node=r;
  i->next=NULL;
  if (root==NULL)
    root=i;
  else {
    Nodo *n = root;
    while (n->next!=NULL)
      n=n->next;
    n->next = i;
  }
  num_nodi++;
}

Vector2i get(int index, bool del) {
  if (index>=num_nodi) return NULL;
  Nodo *t = root;
  Nodo *previous=NULL;
  for (int i=0; i<index; i++){
      previous=t;
      t=t->next;
  }
  Vector2i temp = t->node;
  if (del) {
    if (t == root)
      root=t->next;
    else
      prevoius->next = t->next;
    delete t;
    num_nodi--;
  }
  return temp;
}

Vector2i get(int x, int y, bool del) {
  Noto *t = root;
  Nodo *previous=NULL;

  if (t == NULL) return NULL;

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

  return NULL;
}

void clean() {
  while (num_nodi!=0)
    get(0, true);
}
