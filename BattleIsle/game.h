/*
 * Author: Manuel
 * Version: 0.1
 * Datum 30.12.2017
 *
 * Author: Manuel
 * Version: 0.2
 * Datum: 04.01.2018
 *
 * Author: Manuel
 * Version: 0.3
 * Datum: 12.01.2018
 * Kommentar: Buttons hinzugefuegt
 *
 * Author: Lucas
 * Version: 0.4
 * Datum 14.01.2018
 * Kommentare: ProcessSelection bearbeitet, Dijkstra gefixed, reset funktion geändert, Player cameFrom und current_cost eingefügt,
 *              ChangePhase & Move Button Implementiert, Hilfsfunktionen  eingefügt
 *
<<<<<<< HEAD
 * Author: Miguel
 * Version: 0.5
 * Datum: 23.01.2018
 * Kommentar: getter und setter für targetCache, camefrom und costs. Dijkstra muss jetzt ein HexagonMatchfield* uebergeben bekommen
 * 
=======
 * Autho: Manuel
 * Version: 0.5
 * Datum: 24.01.2018
 * Kommentar:
 * Folgende Probleme gibt es noch:
 * -    Minimap: Laedt man ein Save Game so funktioniert die Minimap nicht korrekt. Das Problem entsteht, wenn auf die Minimap geklickt wird um
 *      wieder zum Spiel zurueck zu gelangen. Das ganze ensteht dadurch, dass das entsprechende Hexagon nicht aktiv ist (siehe in das
 *      mousePressEvent von HexagonMatchfield. Dort liefert die Funktion isActive() ein true zurueck, auch wenn die Minimap aktuell
 *      angezeigt wird, was ich nicht verstehe)
 *
 * -    Resize: Wird ein Spiel ueber das Menue beendet und ein neues Spiel gestartet, passt sich das Fenster nicht angemessen an.
>>>>>>> LucasBearbeitung
 * */

#ifndef GAME_H
#define GAME_H

#include "options.h"
#include "button.h"

#include "hexagonmatchfield.h"
#include "gamewidget.h"
#include "staticunit.h"
#include "dynamicunit.h"
#include "round.h"
#include "ki.h"

#include <vector>
#include <QObject>
#include <QVector3D>
#include <QGraphicsView>
#include <unordered_set>

class GameWidget; //Vorwärtsdeklaration
class HexagonMatchfield;
class Unit;
class Button;
class KI;


class Game : public QObject
{
    Q_OBJECT
private:
    HexagonMatchfield* ptr_hexSelectionCache;
    std::unordered_set<HexagonMatchfield*> set_hexTargetCache;

    std::vector<std::vector<HexagonMatchfield*>> vec_hexGameGrid;
    std::vector<std::vector<Unit*>> vec_unitGrid;
    std::vector<Button*> vec_buttonMenueBar;

    Options* ptr_options;
    GameWidget* ptr_gameWidget;

    std::map<HexagonMatchfield*, HexagonMatchfield*> map_hexCameFrom;
    std::map<HexagonMatchfield*, int> map_hexCurrentCost;

    Player* ptr_playerOne;
    Player* ptr_playerTwo;
    Player* ptr_playerActive;
    Round* ptr_roundCurrent;

    static std::vector<QPoint> vec_qpointOddNeighbors;
    static std::vector<QPoint> vec_qpointEvenNeighbors;

    bool bool_menueView;

	//für ki
    KI* ptr_gameKI;

public:
    Game(Options* init_options, GameWidget* ptr_gameWid);
    Game(QString, GameWidget*);
    ~Game();
    //Game(QString pathToSaveFile);
    void saveGame();
    void endGame();
    void Dijkstra(HexagonMatchfield*, int = 1);
    bool loadMapForNewGame(QString);
    bool readSaveGame(QString);
    bool loadMapFromSaveGame(QString);

    /* Nicht im UML-Diagramm, sind nur eine Hilfsfunktionen für processSelection*/
    void resetHexMatchfield();
    void resetTargetCache();
    void moveUnitTo(HexagonMatchfield*);    //Einheit auf anderes Hex verlegen
    void showNeighbors(HexagonMatchfield*); //Nachbarn markieren -> später range markieren
    void calculateTargets(HexagonMatchfield*, int);
    void setFogOfWar();
    void showPath(HexagonMatchfield *);     //Weg einer bewegung anzeigen
    void checkUnitGrid();                   //Prüfen ob eine Einheit gestorben ist
    void checkWinCondition();               //Prüfen ob ein spieler gewonnen hat
    void resetUnits();
    void countUnits();
    int offset_distance(QPoint, QPoint);
    void serialize(QTextStream&);
    void changeButtonPixmap();

	//fuer ki
    std::unordered_set<HexagonMatchfield*> getTargetCache() const;
	HexagonMatchfield* getCamefrom_Hex(HexagonMatchfield*);
	int getCurrentCost_Int(HexagonMatchfield*);
    void autoplayKi();
    GameWidget *getPtr_gameWidget() const;
    void addUnitToGrid(Unit*, int, int);

private:
    QVector3D oddqToCube(QPoint);   //Für Referenzen betrachte https://www.redblobgames.com/grids/hexagons/#distances , Kapitel: Distance
    QPoint cubeToOddq(QVector3D);
    int cube_distance(QVector3D, QVector3D);
    void createRandomMap();
    void createButtons();
    Unit* createUnitFromType(QString, QString, Player*);
    Unit* readUnitFromStream(QTextStream&);
    void loadInventory(QTextStream&, Unit*);
    void updateLabels();
    void autoRepairUnits();

    /*Hilfsfunktionen Ende*/

public slots:
    void processSelection(HexagonMatchfield* selection);
    void buttonPressedMove();
    void buttonPressedAction();
    //void buttonPressedInformation();
    void buttonPressedMap();
    void buttonPressedMenue();
    void buttonPressedChangePhase();
    void buttonPressedZoomIn();
    void buttonPressedZoomOut();
    void SLOT_MenueButtonSelected(int);
    void SLOT_checkStateOfButtons();
signals:
    void gameOver();

};
class Compare
{
public:
    bool operator ()(std::pair<HexagonMatchfield *, int> a, std::pair<HexagonMatchfield *, int> b);
};

#endif // GAME_H
