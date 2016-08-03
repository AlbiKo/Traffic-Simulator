#include <SFML/Graphics.hpp>

using namespace Vector2i_List;

struct Nodo {
   sf::Vector2i node;
   Nodo * next;
}

class Vector2i_List {

private:
  Nodo *root;
  int num_nodi;

public:
  Vector2i_List();
  Vector2i_List(sf::Vector2i r);
  void insert(sf::Vector2i r);
  Vector2i get(int index, bool del);
  Vector2i get(int x, int y, bool del);
  void clean();

}
