#ifndef DIREZIONATORE_INCLUDE
#define DIREZIONATORE_INCLUDE

#include "Direzione.h"
#include "Direzione_List.h"
#include "enum_toInt.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

using namespace sf;
/**
*	Costituisce una sorta di interfaccia alla lista di Direzioni, 
*	regolando in modo leggermente diverso le operazioni della lista.
*
*	La lista non avrà mai più di quattro elementi (le quattro direzioni), i quali sono tutti distinti.
*	Si aggiunge la possibilità di estrarre casualmente un elemento della lista.
*/
class Direzionatore
{
public:
	/** La lista viene inizializzata con tutte e quattro le direzioni. */
	Direzionatore();

	/** La lista viene cancellata. */
	~Direzionatore();

	/** Si estra casualmente un elemento dalla lista e lo si restituisce. Se vuota restituisce la direzione nulla. */
	Direzione estraiDirezione();

	/** Inserisce la direzione passata come parametro solo se non è già presente nella lista e se non è la direzione nulla.
	*
	*	@param d Direzione da inserire.
	*/
	void ammettiDirezione(Direzione d);

	/** Elimina dalla lista la direzione passata come parametro.
	*
	*	@param d Direzione da eliminare.
	*/
	void escludiDirezione(Direzione d);


	/** Elimina dalla lista le direzioni non ammissibili:
	*	- Quella da cui sta arrivando.
	*	- Quella opposta a quella che porta alla destinazione.
	*	- Se ci si trova a ridosso dei bordi, quelle che portano fuori dalla mappa.
	*
	*	@param startPos Posizione di riferimento.
	*	@param endPos Posizione della destinazione.
	*	@param prevDir Direzione estratta precedentemente.
	*	@param numBlocchi Dimensioni in blocchi della mappa.
	*/
	void escludiDirezioni(Vector2i startPos, Vector2i endPos, Direzione prevDir, Vector2i numBlocchi);
	
	/** Svuota la lista e inserisce tutte e quattro le direzioni .*/
	void ripristina();

	/** Restituisce il numero di elementi della lista. */
	inline int count()
	{
		return dl.count();
	}
private:
	/** Lista di ::Direzione su cui si lavora. */
	Direzione_List dl;

	/** Elimina dalla lista le direzioni non ammissibili in base a dove si trovano
	*	la posizione di riferimento e quella di destinazione (posizionata sul bordo superiore o inferiore):
	*	- Quella da cui sta arrivando.
	*	- Quella opposta che porta alla destinazione.
	*	- Se ci si trova a ridosso dei bordi, quelle che portano fuori dalla mappa.
	*
	*	@param startPos Posizione di riferimento.
	*	@param endPos Posizione della destinazione.
	*	@param border Coordinate che rappresentano il bordo su cui si trova la destinazione.
	*	@param oppositeDir Direzione opposta a quella che porta alla destinazione.
	*/
	void escludiDirezioniX(Vector2i startPos, Vector2i endPos, Vector2i border, Direzione oppositeDir);
};

#endif // DIREZIONATORE_INCLUDE