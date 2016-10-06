#include "Blocco.h"
#include "Direzione.h"
#include <SFML/Graphics.hpp>

using namespace sf;

/** La classe ::Curva rappresenta il blocco di tipo Curva, istruendo alle macchine il momento di svolta.
*	
*	In base al tipo di curva viene scelta la texture appropriata.
*/
class Curva: public Blocco
{

private:
	/** Posizioni in cui i veicoli devono cambiare direzione. */
	Vector2i pos1,	// Posizionato nel quadrante in alto a sinistra.
		pos2,		// Posizionato nel quadrante in alto a destra.
		pos3,		// Posizionato nel quadrante in basso a sinistra.
		pos4;		// Posizionato nel quadrante in basso a destra.

	#ifdef DEBUG_MODE
		/** Indicatori visivi di debug delle posizioni di cambio direzione della macchina. */
		RectangleShape shape_1, shape_2, shape_3, shape_4;
	#endif // DEBUG_MODE
public:
	/**	Costruttore che inizializza una curva generica da sinistra verso l'alto, al blocco (0,0). */
	Curva();

	/**	Costruttore che inizializza un blocco di tipo curva alle coordinate e con la direzione passate come parametro.
	*
	*	@param rowIndex Indice della riga in cui collocare il blocco di tipo Curva.
	*	@param columnIndex Indice della colonna in cui collocare il blocco di tipo Curva.
	*	@param tipo Tipo di blocco da piazzare (indica la direzione che deve avere la curva).
	*/
	Curva(int rowIndex, int columnIndex, TipoBlocco tipo);

	/**	Cambia il verso della curva.
	*	
	*	@param verso Direzione che dovrà avere la curva.
	*/
	void cambiaVerso(TipoBlocco verso);
	
	/** Disegna il blocco. 
	*	Se il debug è attivo disegna anche i punti di cambio direzione.
	*
	*	@param widget Finestra su cui viene disegnato il blocco.
	*/
	void draw(RenderWindow &widget);
	

	/**	Restituisce la direzione che deve prendere la macchina in prossimità di una curva.
	*
	*	@param pos Posizione attuale della macchina.
	*/
	Direzione getChangeDir(Vector2f pos);
};
