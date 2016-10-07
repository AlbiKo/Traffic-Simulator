#include "Blocco.h"
#include <SFML/Graphics.hpp>

using namespace sf;

/** La classe ::Rettilineo rappresenta il blocco di tipo Rettilineo.
*	
*	In base all'orientamento del rettilineo, viene scelta la texture appropriata.
*/
class Rettilineo: public Blocco
{
public:
	/**	Costruttore che inizializza un rettilineo generico orientato in orizzontale, al blocco (0,0). */
	Rettilineo();

	/**	Costruttore che inizializza un rettilineo alle coordinate e con orientamento passati come parametro.
	*
	*	@param rowIndex Indice della riga in cui collocare il blocco di tipo Curva.
	*	@param columnIndex Indice della colonna in cui collocare il blocco di tipo Curva.
	*	@param tipo Tipo di blocco da piazzare (indica l'orientamento del rettilineo).
	*/
	Rettilineo(int rowIndex, int columnIndex, TipoBlocco tipo);

	/**	Cambia il verso del rettilineo.
	*	Se viene passato un verso non valido (diverso da Verticale o Orizzontale) viene forzato l'orizzontale.
	*
	*	@param verso Verticale/Orizzontale.
	*/
	void cambiaVerso(TipoBlocco verso);
	
	/** Disegna il blocco.
	*
	*	@param widget Finestra su cui viene disegnato il blocco.
	*/
	void draw(RenderWindow &widget);
};