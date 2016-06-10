#include "../header/Nodo.h"
#include "../header/Debug.h"


Nodo::Nodo()
{
    pos.x 		 = 0;
    pos.y 		 = 0;
    numAdiacenze = 0;

    for (int i = 0; i < 4; i++)
        adiacenze[i] = NULL;

	D2(std::cout<<"Nodo in posizione" <<pos.x <<"," <<pos.y <<std::endl);
}

Nodo::Nodo(Vector2i startPos)
{
    Nodo();
    pos = startPos;

    D1(std::cout<<"Nodo in posizione" <<pos.x <<"," <<pos.y <<std::endl);
}

Nodo::setPos(int x, int y)
{

}
