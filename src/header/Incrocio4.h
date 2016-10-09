#include "Incroci.h"
#include <SFML/Graphics.hpp>
#include "Direzione.h"

using namespace sf;

/** La classe ::Incrocio4 rappresenta il blocco con 4 intersezioni. */
class Incrocio4: public Incroci
{

public:
	/** Costruttore di default che posiziona l'incrocio alle coordinate (0,0). */
	Incrocio4() ;
	
	/** Costruttore che inizializza un incrocio alle coordinate passate per parametro.
	*
	*	@param rowIndex Indice della riga.
	*	@param columnIndex Indice della colonna.
	*/
	Incrocio4(int rowIndex, int columnIndex);

	/** Disegna il blocco. 
	*	Se il debug è attivo disegna anche i punti di cambio direzione.
	*
	*	@param widget Finestra su cui viene disegnato il blocco.
	*/
	void draw(RenderWindow &widget);

};