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

	/** Inizializzazione della generazione della strada che comincia in startPos e conclude in endPos.
	*	A seconda di dove si trova la partenza, si inizializza la posizione corrente,
	*	che sarà immediatamente a fianco, e la direzione estratta, che sarà sinistra o destra.
	*	A seconda di dove si trova, l'arrivo (sorgente posizionata in alto o in basso) viene spostato
	*	alla cella superiore o inferiore.
	*
	*	@param startPos Posizione nella matrice della sorgente di partenza, cioè collocata a sinistra o a destra
	*	@param endPos Parametro di uscita. Posizione nella matrice della sorgente di arrivo, cioè collocata sopra o sotto
	*	@param currentPos Posizione nella matrice attuale corrente
	*	@param prevDir Direzione estratta precedente
	*	@return Restituisce la nuova posizione corrente, la nuova posizione di arrivo e la direzione precedente
	*/
	void initGeneratingRoute(Vector2i startPos, Vector2i& endPos, Vector2i& currentPos, Direzione& prevDir);
	void applyRouteBlock(Vector2i& currentPos, Direzione& prevDir, Direzione currentDir, TipoBlocco tipo);
	void autocompleteRoute(Vector2i currentPos, Vector2i endPos, Direzione prevDir);
	
	/** Si stabilisce il nuovo tipo di tale blocco basandosi sul valore delle ultime due direzioni estratte.
	*	Vengono verificati evenutali conflitti con blocchi già presenti sulla mappa, sorgenti comprese.
	*
	*	@param currentPos Posizione del blocco attuale nella matrice
	*	@param prevDir Direzione estratta precedente
	*	@param currentDir Direzione attualmente estratta
	*	@return Tipo del blocco risultante dalla combinazione delle due direzioni
	*/
	TipoBlocco mergeRouteBlocks(Vector2i currentPos, Direzione prevDir, Direzione currentDir);

	/** Assumendo che il blocco attuale sia vuoto, si stabilisce il nuovo tipo di tale blocco
	*	basandosi sul valore delle ultime due direzioni estratte
	*
	*	@param prevDir Direzione estratta precedente
	*	@param currentDir Direzione attualmente estratta
	*	@return Tipo del blocco risultante dalla combinazione delle due direzioni
	*/
	TipoBlocco mergeEmptyRouteBlock(Direzione prevDir, Direzione currentDir);

	/**	Controlla se mettendo un blocco di un certo tipo nella posizione
	*	specificata si crea un conflitto con eventuali sorgenti.
	*
	*	@param currentPos Coordinate del blocco nella matrice
	*	@param tipo Tipo di blocco che si vuole posizionare in currentPos
	*	@return Restituisce il tipo di blocco che risolve il conflitto con
	*	le eventuali sorgenti. Se non ci sono conflitti, restituisce il parametro
	*	tipo.
	*/
	TipoBlocco checkSourceRouteBlock(Vector2i currentPos, TipoBlocco tipo);

	/**	Controlla se mettendo una curva di un certo tipo nella posizione specificata
	*	si crea un conflitto con eventuali sorgenti basandosi sulla posizione
	*	dell'angolo che può creare due conflitti.
	*	
	*	@param currentPos Coordinate del blocco nella matrice
	*	@param cornerPos Coordinate dell'angolo nella matrice
	*	@param offset Indica quanto ci si deve spostare rispetto al blocco corrente
	*		per controllare la presenza`di una sorgente
	*	@param base Tipo del blocco di partenza
	*	@param typeX Tipo del blocco da assegnare se si individua un conflitto sull'asse X
	*	@param typeY Tipo del blocco da assegnare se si individua un conflitto sull'asse Y
	*
	*	@return Restituisce il tipo di blocco che risolve il conflitto con
	*	le eventuali sorgenti. Se non ci sono conflitti, restituisce il parametro
	*	base.
	*/
	TipoBlocco checkSourceCurveRouteBlock(Vector2i currentPos, Vector2i cornerPos, Vector2i offset, TipoBlocco base, TipoBlocco typeX, TipoBlocco typeY);
public:
	/** Crea e inizializza la matrice che rappresenta la mappa.
	*	Inizialmente, tutti i blocchi presenti nella matrice saranno vuoti (::TipoBlocco:EMPTY)
	*/
    Mappa();

	/** Disegna la mappa rappresentata dalla matrice
	*	@param widget Window SFML in cui andrà disegnata la mappa
	*/
    void draw(RenderWindow &widget);

	/** Genera la mappa
	*/
    void generate();
};
