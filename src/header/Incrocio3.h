#include "Incroci.h"
#include <SFML/Graphics.hpp>

using namespace sf;

/** La classe ::Incrocio4 rappresenta il blocco con 3 intersezioni. */
class Incrocio3: public Incroci
{

private:
	/** Imposta la direzione in cui non possono andare le macchine. */
	void setWrongDir();

public:

	/** Costruttore di default che posiziona l'incrocio alle coordinate (0,0) con direzione a T rovesciata. */
	Incrocio3() ;
	
	/** Costruttore che inizializza un incrocio alle coordinate passate per parametro, con direzione passata per parametro.
	*
	*	@param rowIndex Indice della riga.
	*	@param columnIndex Indice della colonna.
	*	@param tipo Tipo di incrocio.
	*/
	Incrocio3(int rowIndex, int columnIndex, TipoBlocco tipo);

	/**	Cambia il verso dell'incrocio.
	*	
	*	@param verso Direzione che dovrà avere la l'incrocio.
	*/
	void cambiaVerso(TipoBlocco verso);

	/** Disegna il blocco. 
	*	Se il debug è attivo disegna anche i punti di cambio direzione.
	*
	*	@param widget Finestra su cui viene disegnato il blocco.
	*/
	void draw(RenderWindow &widget);
};