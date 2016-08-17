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
	D2(PRINT("Ammetto nel Direzionatore: "<< toString(d)));
	if (d != Direzione::ND && dl.get(d, false) == Direzione::ND)
		dl.insert(d);
}

void Direzionatore::escludiDirezione(Direzione d)
{
	D2(PRINT("Escludo direzione: " << toString(d)));
	dl.get(d, true);
}

void Direzionatore::escludiDirezioni(Vector2i startPos, Vector2i endPos, Direzione prevDir, Vector2i numBlocchi)
{
	D1(PRINT("\nEscludo direzioni.."));
	D3(PRINT("Partenza: " <<startPos.x <<", " <<startPos.y));
	D3(PRINT("Destinazione: " << endPos.x << ", " << endPos.y));
	D3(PRINT("Prev dir: " <<toString(prevDir)));

	ripristina();
	escludiDirezione(getDirOpposta(prevDir));

	//Si escludono direzioni se la destinazione è in alto..
	if (endPos.y == 0)
		escludiDirezioniX(startPos, endPos, Vector2i(numBlocchi.x - 2, 1), Direzione::GIU);
	//.. o in basso
	if (endPos.y == numBlocchi.y - 1)
		escludiDirezioniX(startPos, endPos, Vector2i(numBlocchi.x - 2, numBlocchi.y - 2), Direzione::SU);


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

void Direzionatore::escludiDirezioniX(Vector2i startPos, Vector2i endPos, Vector2i border, Direzione oppositeDir)
{
	escludiDirezione(oppositeDir);

	//Se ci si trova a ridosso della destinazione o alla sua sinistra
	if (startPos.x == 1 || (startPos.x <= endPos.x && startPos.y == border.y))
		escludiDirezione(Direzione::SX);

	//Se ci si trova a ridosso della destinazione o alla sua destra
	if (startPos.x == border.x|| (startPos.x >= endPos.x && startPos.y == border.y))
		escludiDirezione(Direzione::DX);

	//Se ci si trova sul bordo ma non a ridosso della destinazione
	if (startPos.y == border.y && startPos.x != endPos.x)
		escludiDirezione(getDirOpposta(oppositeDir));
}
