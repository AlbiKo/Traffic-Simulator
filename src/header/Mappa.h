#ifndef MAPPA_INCLUDE
#define MAPPA_INCLUDE

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
#include "Blocco_List.h"
#include <ctime>

using namespace sf;

/**	La classe ::Mappa descrive l'area di gioco rappresentata attraverso una matrice
*	di blocchi.
*	La mappa viene generata casualmente passando attraverso tre fasi:
*
*	1) Posizionamento delle sorgenti, ovvero i blocchi da cui le macchine possono 
*		entrare/uscire, sui bordi.\n
*	2) Si collegano le sorgenti fra loro: ogni sorgente posizionata sul bordo 
*		sinistro o destro viene collegata con una sorgente posizionata sul bordo 
*		in alto o in basso scelta casualmente.\n
*	3) Se al termine della fase due risultano delle sorgenti che non sono 
*		collegate a qualcosa, si cerca di collegare tali sorgenti con la sorgente 
*		a loro più vicina. In caso di insuccesso la sorgente viene eliminata.\n
*	\n
*	Inoltre, la mappa fornisce l'interfaccia per aggiornare lo stato dei semafori e per il controllo delle collisioni
*	fra macchine all'interno di un blocco.
*/

class Mappa
{
private:

	/** Numero massimo di tentativi per costruire una strada. */
	const int MAX_TENTATIVI = 16;

		/** Numero di blocchi sull'asse orizzontale. */
	int blocchiX, 
		/** Numero di blocchi sull'asse verticale. */
		blocchiY;

	/** Matrice di puntatori a oggetti di tipo Blocco. 
	*	Rappresenta la mappa e il blocco nella matrice all'indice 0,0 indica
	*	il blocco sull'angolo in alto a sinistra.
	*/
	Blocco *** blocchi;

	/** Lista con le posizioni delle sorgenti. */
	Vector2i_List sourceList;

	/** Lista con le posizioni degli incroci. */
	Vector2i_List crossList;

	/** Carica le texture dei blocchi nell'apposita matrice. */
	void loadTextures();

	/**	Carica la texture specificata dal tipo di blocco su cui applicarla e 
	*	dal nome del file corrispondente nella cartella media/img.
	*
	*	@param tipo Tipo di blocco.
	*	@param nome Nome del file della texture.
	*/
	void loadTexture(TipoBlocco tipo, std::string nome);

	void initMap();

	Vector2i getNumBlocchi();

	/** Pulisce la matrice, azzerando tutti i percorsi, le sorgenti e i semafori. */
	void clean();
	
	/** Cambia il blocco passato come parametro deallocandolo e riallocandolo come 
	*	oggetto nel tipo specificato. Se il blocco è già del tipo specificato, 
	*	non avviene alcuna riallocazione.
	*
	*	@param blocco Puntatore del blocco da modificare.
	*	@param tipo Nuovo tipo del blocco.
	*/
	void cambiaTipoBlocco(Blocco * &blocco, TipoBlocco tipo);

	/** Estrae casualemente un valore booleano. */
	bool randomBool();

	/** Le sorgenti vengono posizonate sui quattro bordi casualmente con 
	*	il vincolo che due sorgenti devono avere almeno due blocchi che le separano.
	*/
	void generateSources();

	/** Si posizionano casualmente le sorgenti sul bordo indicato dalle 
	*	coordinate dell'angolo che lo tocca passate e dal tipo di bordo, che può 
	*	essere verticale o orizzontale.
	*	In base al tipo di bordo, si capisce quale delle due coordinate 
	*	deve rimanere fissa.
	*
	*	@param x Coordinata x dell'angolo relativo al bordo.
	*	@param y Coordinata y dell'angolo relativo al bordo.
	*	@param vertical Tipo di bordo. Se true si intende un bordo verticale.
	*/
	void generateSource(int x, int y, bool vertical);

	/** Si elimina dalla lista e dalla matrice la sorgente selezionata.
	*
	*	@param source Coordinate della sorgente.
	*/
	void deleteSource(Vector2i source);

	/** Si controlla se ci sono delle sorgenti, intese sia come partenze sia come 
	*	destinazioni, che non sono collegate a nulla.
	*
	*	@param starts Lista con le coordinate delle partenze.
	*	@param ends Lista con le coordinate delle destinazioni.
	*/
	void checkUnlinkedSources(Vector2i_List& starts, Vector2i_List& ends);

	/** Individua e restituisce la sorgente più vicina a quella passata come parametro
	*
	*	@param source Sorgente riferimento
	*	@return Sorgente più vicina a source
	*/
	Vector2i getNearestSource(Vector2i source);

	/** Collega le sorgenti fra loro distinguendole in due categorie: 
	*	partenze, ai lati sinistro e destro, destinazioni, in alto e in basso.
	*	Ogni partenza viene collegata ad una destinazione scelta casualmente. 
	*	Se al termine risultano sorgenti non collegate si tenta di collegarle alle loro sorgenti 
	*	più vicine. In caso di insuccesso, vengono eliminate.
	*/
	void generateRoutes();

	/** Genera casualmente la strada che inizia in startPos e si conclude in endPos, 
	*	escludendo la possibilità che ci siano due incroci vicini o che si verifichi 
	*	un zig-zag (questo per evitare una eccessiva seghettatura del percorso).
	*
	*	@param startPos Coordinate della sorgente di partenza.
	*	@param endPos Coordinate della sorgente di partenza.
	*	@return True se è riuscito a collegare le due sorgenti entro il numero di tentativi prestabiliti. False altrimenti.
	*/
	bool generateRoute(Vector2i startPos, Vector2i endPos);

	/** Inizializzazione della generazione della strada che comincia in startPos e 
	*	conclude in endPos. A seconda di dove si trova la partenza, si inizializza la posizione
	*	corrente, che sarà immediatamente a fianco, e la direzione estratta, che sarà 
	*	sinistra o destra.
	*
	*	@param startPos Posizione nella matrice della sorgente di partenza, cioè collocata a sinistra o a destra.
	*	@param currentPos Posizione nella matrice attuale corrente.
	*	@param prevDir Direzione estratta precedente.
	*	@param prevBlock.
	*	@return Restituisce la nuova posizione corrente e la direzione precedente.
	*/
	void initGeneratingRoute(Vector2i startPos, Vector2i& currentPos, Direzione& prevDir, TipoBlocco& prevBlock);

	/**	Si inserisce nella lista di blocchi un blocco del tipo e alle coordinate specificate.
	*	Poi si aggiorna la posizione corrente secondo la direzione corrente.
	*
	*	@param bloccoList Percorso attualmente costruito.
	*	@param currentPos Posizione del blocco corrente. Prima dell'aggiornamento indica le coordinate del blocco da inserire mentre dopo indica la posizione del blocco di cui si dovrà stabilirne il tipo.
	*	@param prevDir Direzione estratta precedentemente (Parametro di uscita).
	*	@param currentDir Direzione estratta.
	*	@param tipo Tipo del blocco da inserire.
	*/
	void nextStepRouteBlock(Blocco_List & bloccoList, Vector2i& currentPos, Direzione& prevDir, Direzione currentDir, TipoBlocco tipo);

	/** Si applica il percorso individuato sulla matrice.
	*
	*	@param bloccoList Percorso da applicare.
	*/
	void applyRouteBlocks(Blocco_List & bloccoList);

	/** Controlla se intorno al blocco, se è un incrocio, si trovano altri incroci. 
	*	Il controllo si basa sulla matrice.
	*
	*	@param currentPos Coordinate del blocco.
	*	@param tipo Tipo del blocco.
	*	@return True se il blocco è un incrocio e intorno a lui si trovano altri incroci. False altrimenti.
	*/
	bool checkAdjacentCross(Vector2i currentPos, TipoBlocco tipo);

	/** Controlla se intorno al blocco, se è un incrocio, si trovano altri incroci. 
	*	Il controllo si basa sul percorso.
	*
	*	@param bloccoList Percorso.
	*	@param currentPos Coordinate del blocco.
	*	@param tipo Tipo del blocco.
	*	@return True se il blocco è un incrocio e intorno a lui si trovano altri incroci. False altrimenti.
	*/
	bool checkAdjacentCross(Blocco_List& bloccoList, Vector2i currentPos, TipoBlocco tipo);

	/**	Controlla se si è andato a formare un zig-zag.
	*
	*	@param prevBlock Blocco precedentemente piazzato.
	*	@param currentBlock	Blocco piazzato.
	*	@return True se si è formato un zig-zag. False altrimenti.
	*/
	bool checkZigZag(TipoBlocco prevBlock, TipoBlocco currentBlock);

	/** Si stabilisce il nuovo tipo di tale blocco basandosi sul valore delle ultime
	*	due direzioni estratte. Vengono verificati evenutali conflitti con blocchi 
	*	già presenti sulla mappa, sorgenti comprese.
	*
	*	@param currentPos Posizione del blocco attuale nella matrice.
	*	@param prevDir Direzione estratta precedente.
	*	@param currentDir Direzione attualmente estratta.
	*	@return Tipo del blocco risultante dalla combinazione delle due direzioni.
	*/
	TipoBlocco mergeRouteBlocks(Vector2i currentPos, Direzione prevDir, Direzione currentDir);

	/** Assumendo che il blocco attuale sia un rettilineo, si stabilisce il nuovo tipo 
	*	di tale blocco basandosi sul valore delle ultime due direzioni estratte e 
	*	sulle direzioni derivanti dal tipo di blocco già presente.
	*
	*	@param tipo Tipo di blocco attuale.
	*	@param prevDir Direzione estratta precedente.
	*	@param currentDir Direzione attualmente estratta.
	*	@param rectDir Direzione parallela al rettilineo.
	*	@param perpDir Direzione perpendicolare al rettilineo.
	*	@param perpBlock Tipo di blocco da assegnare se si entra/esce dal rettilineo da/verso perpDir.
	*	@param nonPerpBlock Tipo di blocco da assegnare se si entra/esce dal rettilineo da/verso la direzione opposta a perpDir.
	*	@return Tipo del blocco risultante dalla combinazione delle due direzioni e dal tipo di blocco attualmente presente.
	*/
	TipoBlocco mergeRectRouteBlock(TipoBlocco tipo, Direzione prevDir, Direzione currentDir, Direzione rectDir, Direzione perpDir, TipoBlocco perpBlock, TipoBlocco nonPerpBlock);

	/** Assumendo che il blocco attuale sia una curva, si stabilisce il nuovo tipo di tale 
	*	blocco basandosi sul valore delle ultime due direzioni estratte e sulle due direzioni 
	*	del blocco mancanti per essere un incrocio a 4.
	*
	*	@param tipo Tipo di blocco attuale.
	*	@param prevDir Direzione estratta precedente.
	*	@param currentDir Direzione attualmente estratta.
	*	@param missingDirX Direzione mancante al bloccco dell'asse X per essere un incrocio a 4.
	*	@param missingDirY Direzione mancante al bloccco dell'asse Y per essere un incrocio a 4.
	*	@param typeX Tipo di blocco con lato piatto sull'asse X da assegnare se si verificano i casi appositi.
	*	@param typeY Tipo di blocco con lato piatto sull'asse Y da assegnare se si verificano i casi appositi.
	*	@return Tipo del blocco risultante dalla combinazione delle due direzioni e dal tipo di blocco attualmente presente.
	*/
	TipoBlocco mergeCurveRouteBlock(TipoBlocco tipo, Direzione prevDir, Direzione currentDir, Direzione missingDirX, Direzione missingDirY, TipoBlocco typeX, TipoBlocco typeY);


	/** Assumendo che il blocco attuale sia un incrocio a 3, si stabilisce il nuovo tipo 
	*	di tale blocco basandosi sul valore delle ultime due direzioni estratte e sulla 
	*	direzione del blocco mancante per essere un incrocio a 4.
	*
	*	@param tipo Tipo di blocco attuale.
	*	@param prevDir Direzione estratta precedente.
	*	@param currentDir Direzione attualmente estratta.
	*	@param missingDir Direzione mancante al bloccco per essere un incrocio a 4.
	*	@return Tipo del blocco risultante dalla combinazione delle due direzioni e dal tipo di blocco attualmente presente.
	*/
	TipoBlocco mergeCross3RouteBlock(TipoBlocco tipo, Direzione prevDir, Direzione currentDir, Direzione missingDir);
	
	/** Assumendo che il blocco attuale sia vuoto, si stabilisce il nuovo tipo di tale blocco
	*	basandosi sul valore delle ultime due direzioni estratte.
	*
	*	@param prevDir Direzione estratta precedente.
	*	@param currentDir Direzione attualmente estratta.
	*	@return Tipo del blocco risultante dalla combinazione delle due direzioni.
	*/
	TipoBlocco mergeEmptyRouteBlock(Direzione prevDir, Direzione currentDir);

	/**	Controlla se mettendo un blocco di un certo tipo nella posizione
	*	specificata si crea un conflitto con eventuali sorgenti.
	*
	*	@param currentPos Coordinate del blocco nella matrice.
	*	@param tipo Tipo di blocco che si vuole posizionare in currentPos.
	*	@return Restituisce il tipo di blocco che risolve il conflitto con
	*	le eventuali sorgenti. Se non ci sono conflitti, restituisce il parametro
	*	tipo.
	*/
	TipoBlocco checkSourceRouteBlock(Vector2i currentPos, TipoBlocco tipo);

	/**	Controlla se mettendo una curva di un certo tipo nella posizione specificata
	*	si crea un conflitto con eventuali sorgenti basandosi sulla posizione
	*	dell'angolo che può creare due conflitti.
	*	
	*	@param currentPos Coordinate del blocco nella matrice.
	*	@param cornerPos Coordinate dell'angolo nella matrice.
	*	@param offset Indica quanto ci si deve spostare rispetto al blocco corrente
	*		per controllare la presenza`di una sorgente.
	*	@param base Tipo del blocco di partenza.
	*	@param typeX Tipo del blocco da assegnare se si individua un conflitto sull'asse X.
	*	@param typeY Tipo del blocco da assegnare se si individua un conflitto sull'asse Y.
	*
	*	@return Restituisce il tipo di blocco che risolve il conflitto con
	*	le eventuali sorgenti. Se non ci sono conflitti, restituisce il parametro
	*	base.
	*/
	TipoBlocco checkSourceCurveRouteBlock(Vector2i currentPos, Vector2i cornerPos, Vector2i offset, TipoBlocco base, TipoBlocco typeX, TipoBlocco typeY);
public:
	/** Crea e inizializza la matrice che rappresenta la mappa.
	*	Inizialmente, tutti i blocchi presenti nella matrice saranno vuoti (::TipoBlocco::EMPTY).
	*/
    Mappa();

	/** Disegna la mappa rappresentata dalla matrice.
	*	@param widget Window SFML in cui andrà disegnata la mappa
	*/
    void draw(RenderWindow &widget);

	/** Genera la mappa.	*/
    void generate();

	/**	Restituisce il blocco alle date coordinate nella matrice.
	*
	*	@param rowIndex Indice di riga.
	*	@param columnIndex Indice di colonna.
	*	@return Puntatore del blocco.
	*/
	Blocco * getBlocco(int rowIndex, int columnIndex);

	/** Restituisce il blocco alle date coordinate del punto sulla finestra.
	*
	*	@param pos Coordinate del punto.
	*	@return Puntatore del blocco.
	*/
	Blocco * getBlocco(Vector2i pos);

	/** Restituisce la dimensione della mappa. */
	Vector2i getMapSize();

	/** Copia il contenuto della lista sorgenti nella lista destinataria.
	*
	*	@param dest Lista destinataria.
	*/
	void getSorgenti(Vector2i_List &dest);

	/** Restituisce una sorgente scelta casualmente dalla lista. */
	Vector2i getRandomSource();

	/** Per ogni blocco della mappa si controllano le collisioni fra le 
	*	macchine che sono in intersezione con quel blocco.
	*/
	void checkCarCollision();

	/**Aggiorna tutti i semafori allo stato successivo. */
	void updateSemaphores(bool blockAll);
};
#endif

