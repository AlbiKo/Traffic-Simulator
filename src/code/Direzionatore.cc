#include "../header/Direzionatore.h"
#include "../header/Debug.h"

Direzionatore::Direzionatore()
{
	D2(PRINT("Costruzione Direzionatore.."));

	dl = Direzione_List();
	ripristina();
}

Direzionatore::~Direzionatore()
{
	D2(PRINT("Distruzione Direzionatore.."));
}

Direzione Direzionatore::estraiDirezione()
{
	if (dl.count() == 0)
		return Direzione::ND;

	srand(time(0));
	return dl.get(rand() % dl.count(),false);
}

void Direzionatore::ammettiDirezione(Direzione d)
{
	D2(PRINT("Ammetto nel Direzionatore: "<< toInt(d)));
	if (d != Direzione::ND && dl.get(d, false) == Direzione::ND)
		dl.insert(d);
}

void Direzionatore::escludiDirezione(Direzione d)
{
		D2(PRINT("Escludo direzione: " << toInt(d)));
		dl.get(d, true);
}

/**

	@param prevDir Direzione da cui si è venuti
*/
void Direzionatore::escludiDirezioni(sf::Vector2i partenza, sf::Vector2i destinazione, Direzione prevDir)
{
	D1(PRINT("\nEscludo direzioni.."));
	D1(PRINT("Partenza: " <<partenza.x <<", " <<partenza.y));
	D1(PRINT("Destinazione: " << destinazione.x << ", " << destinazione.y));
	D1(PRINT("Prev dir: " <<toInt(prevDir)));

	ripristina();
	escludiDirezione(prevDir);

	if (partenza.y < destinazione.y)
		escludiDirezione(Direzione::SU);
	else
		escludiDirezione(Direzione::GIU);

}

void Direzionatore::ripristina()
{
	D2(PRINT("Ripristino Direzionatore.."));
	dl.clean();

	dl.insert(Direzione::SU);
	dl.insert(Direzione::GIU);
	dl.insert(Direzione::SX);
	dl.insert(Direzione::DX);
}