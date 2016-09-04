#include "Blocco.h"
#include "Direzione.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Curva: public Blocco
{

private:
	Vector2i pos1, pos2, pos3, pos4;
public:
	Curva() ;
	Curva(int rowIndex, int columnIndex, TipoBlocco tipo);

	void cambiaVerso(TipoBlocco verso);
	void draw(RenderWindow &widget);
#ifdef DEBUG_MODE
	RectangleShape shape_1, shape_2, shape_3, shape_4;
#endif // DEBUG_MODE
	

	/**Restituisce la direzione che deve prendere la macchina in prossimità di una curva.
	*  @param pos Posizione attuale della macchina
	*/
	Direzione getChangeDir(Vector2f pos);
};
