#include <SFML/Graphics.hpp>
#include "TipoBlocco.h"
#include "enum_toInt.h"
#include "Blocco.h"
#include "Rettilineo.h"
#include "Curva.h"
#include "Incrocio3.h"
#include "Incrocio4.h"
#include "Vector2i_List.h"
#include "Direzionatore.h"
#include <ctime>


using namespace sf;

class Mappa
{
private:

	int blocchiX, blocchiY;
	Blocco *** blocchi;
	Vector2i_List sorgenti;

	void loadTextures();
	void clean();
	void cambiaTipoBlocco(Blocco * &blocco, TipoBlocco tipo);

	bool randomBool();
	void generateSources();
	void generateSource(int x, int y, bool vertical);

	/** Collega le sorgenti dei lati sx/dx a quelle in alto e in basso
	*/
	void generateRoutes();

	/**
	*
	*/
	void generateRoute(Vector2i startPos, Vector2i endPos);
	void initGeneratingRoute(Vector2i startPos, Vector2i& endPos, Vector2i& currentPos, Direzione& prevDir);
	void applyRouteBlock(Vector2i& currentPos, Direzione& prevDir, Direzione currentDir, TipoBlocco tipo);
	void autocompleteRoute(Vector2i currentPos, Vector2i endPos, Direzione prevDir);
	TipoBlocco mergeRouteBlocks(Vector2i currentPos, Direzione prevDir, Direzione currentDir);
	TipoBlocco mergeEmptyRouteBlock(Direzione prevDir, Direzione currentDir);

	TipoBlocco checkSourceRouteBlock(Vector2i currentPos, TipoBlocco tipo);
	TipoBlocco checkSourceCurveRouteBlock(Vector2i currentPos, Vector2i cornerPos, Vector2i offset, TipoBlocco base, TipoBlocco typeX, TipoBlocco typeY);
public:
    Mappa();

    void draw(RenderWindow &widget);
    void generate();
};
