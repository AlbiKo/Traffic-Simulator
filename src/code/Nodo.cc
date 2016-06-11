#include "../header/Nodo.h"
#include "../header/Debug.h"


Nodo::Nodo()
{
    pos.x 		 = 0;
    pos.y 		 = 0;
    peso		 = 0;
    numAdiacenze = 0;

    for (int i = 0; i < 4; i++)
        adiacenze[i] = NULL;

	D2(std::cout<<"Nodo: posizione" <<pos.x <<"," <<pos.y <<std::endl);
}

Nodo::Nodo(Vector2i startPos)
{
    Nodo();
    pos = startPos;

    D1(std::cout<<"Nodo: posizione" <<pos.x <<"," <<pos.y <<std::endl);
}

void Nodo::setPos(int x, int y)
{
	D2(std::cout<<"Nodo: vecchia posizione " <<pos.x <<"," <<pos.y <<std::endl);
	D1(std::cout<<"Nodo: nuova posizione " <<x <<"," <<y <<std::endl);

	pos.x = x;
	pos.y = y;
}

Nodo * Nodo::getAdiacenza(int i)
{
	assert(i >= 0 && i <= 3);
	D1(PRINT("Nodo: leggo adiacenza in " <<i));

	if (i < 0 || i > 3)
	{
		D1(PRINT("Nodo: letta adiacenza " <<adiacenze[i]));
		return NULL;
	}

	return adiacenze[i];
}

void Nodo::setAdiacenza(int i, Nodo * nodo)
{
	assert(i >= 0 && i <= 3);
	D1(PRINT("Nodo: scrivo adiacenza in " <<i <<": " <<nodo));

	if (i >= 0 && i <= 3)
		adiacenze[i] = nodo;
}

bool Nodo::setAdiacenza(Nodo * nodo)
{
	int i;
	for (i = 0; i < 4 && adiacenze[i] != NULL; i++);

	if (i == 4)
	{
		D2(PRINT("Nodo: adiacenza "<<nodo <<" non impostata; nessuna posizione libera"));
		return false;
	}
	else
	{
		adiacenze[i] = nodo;
		D1(PRINT("Nodo adiacenza "<<nodo <<"impostata alla posizione " <<i));
		return true;
	}

}
