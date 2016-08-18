#include "Blocco.h"
#include <SFML/Graphics.hpp>
#include "Direzione.h"

using namespace sf;

class Incrocio4: public Blocco
{

private:
	Vector2i pos1, pos2, pos3, pos4;
public:
	Incrocio4() ;
	Incrocio4(int rowIndex, int columnIndex);
	RectangleShape shape_1, shape_2, shape_3, shape_4; //DEBUG

	/**Restituisce la direzione che deve prendere la macchina per essere instradata correttamente.
	*  @param pos Posizione attuale della macchina
	*  @param dir Direzione del nodo successivo all'incrocio
	*/
	Direzione getChangeDir(Vector2f pos, Direzione dir);
};
