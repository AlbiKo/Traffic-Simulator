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
	dl.clean();
}

Direzione Direzionatore::estraiDirezione()
{
	if (dl.count() == 0)
		return Direzione::ND;

	return dl.get(rand() % dl.count(),false);
}

void Direzionatore::ammettiDirezione(Direzione d)
{
	D2(PRINT("Ammetto nel Direzionatore: "<< stampaDir(d)));
	if (d != Direzione::ND && dl.get(d, false) == Direzione::ND)
		dl.insert(d);
}

void Direzionatore::escludiDirezione(Direzione d)
{
	D2(PRINT("Escludo direzione: " << stampaDir(d)));
	dl.get(d, true);
}

void Direzionatore::escludiDirezioni(sf::Vector2i partenza, sf::Vector2i destinazione, Direzione prevDir, sf::Vector2i numBlocchi)
{
	D1(PRINT("\nEscludo direzioni.."));
	D3(PRINT("Partenza: " <<partenza.x <<", " <<partenza.y));
	D3(PRINT("Destinazione: " << destinazione.x << ", " << destinazione.y));
	D3(PRINT("Prev dir: " <<stampaDir(prevDir)));

	ripristina();
	escludiDirezione(getDirOpposta(prevDir));

	//Contollo dove si trova la destinazione rispetto la partenza ed escludo la direzione opposta
	if (partenza.y < destinazione.y)
		escludiDirezione(Direzione::SU);
	else
		escludiDirezione(Direzione::GIU);

	//Escludo le direzioni opportune se mi trovo sui bordi
	if (partenza.x == 1)
		escludiDirezione(Direzione::SX);

	if (partenza.x == numBlocchi.x - 2)
		escludiDirezione(Direzione::DX);

	if (partenza.y == 1)
		escludiDirezione(Direzione::SU);

	if (partenza.y == numBlocchi.y - 2)
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