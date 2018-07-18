/*
 * Author: Manuel
 * Version: 0.1
 * Datum 30.12.2017
 *
 * Author: Manuel
 * Version: 0.2
 * Datum: 04.01.2018
 *
 * Author: Lucas
 * Version: 0.4
 * Datum 14.01.2018
 * Kommentare: ProcessSelection bearbeitet, Dijkstra gefixed, reset funktion geändert, Player cameFrom und map_hexCurrentCost eingefügt,
 *              ChangePhase & Move Button Implementiert, Hilfsfunktionen  eingefügt
 *
 * Author: Miguel
 * Version: 0.5
 * Datum: 23.01.2018
 * Kommentar: getter und setter für targetCache, camefrom und costs. Dijkstra muss jetzt ein HexagonMatchfield* uebergeben bekommen
 * 		Grund für die Aenderung ist die KI.
 *
 * Author: Miguel
 * Version: 0.6
 * Datum: 26.01.2018
 * Kommentar: ki kann sich bewegen, ki kann im menue eingestellt werden
 *
 * Author: Miguel
 * Version: 0.7
 * Datum: 27.01.2018
 * Kommentar: original map eingefügt und alle unit typen ergänzt beim laden.
 * Fehler entfernt der zum spiel absturzt führt wenn targetCache noch gesetzt ist und die ki am zug ist.
 * 
 * Author: Benny, Kevin
 * Version: 0.8
 * Datum: 28.01.2018
 * Kommentar: In process selection bei action einen Parameter hinzugefügt
 * */

#include "game.h"
#include <typeinfo>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QTextCodec>
#include <QTime>
#include <queue>
#include <QElapsedTimer>

using namespace std;

std::vector<QPoint> Game::vec_qpointEvenNeighbors = {QPoint(1,0),QPoint(1,-1),QPoint(0,-1),QPoint(-1,-1),QPoint(-1,0),QPoint(0,1)};
std::vector<QPoint> Game::vec_qpointOddNeighbors = {QPoint(1,1),QPoint(1,0),QPoint(0,-1),QPoint(-1,0),QPoint(-1,1),QPoint(0,1)};


GameWidget *Game::getPtr_gameWidget() const
{
    return ptr_gameWidget;
}

void Game::addUnitToGrid(Unit * newUnit, int posX, int posY)
{
    if(posX > 0 && posX < ptr_gameWidget->getSizeX() && posY > 0 && posY < ptr_gameWidget->getSizeY())
        vec_unitGrid[posX][posY] = newUnit;
}

Game::Game(Options *init_options, GameWidget *ptr_gameWid) :
    ptr_hexSelectionCache(nullptr), ptr_options(init_options), ptr_gameWidget(ptr_gameWid),
    ptr_playerOne(new Player("Eins", 1)), ptr_playerTwo(new Player("Zwei", 2)),
    ptr_playerActive(ptr_playerOne), bool_menueView(false), ptr_gameKI(nullptr)
{
/*Starten eines Spiels mit den Optionen definiert in init_Options*/

    ptr_roundCurrent = new Round(ptr_options->getInt_roundLimit());

    if(!loadMapForNewGame(ptr_options->getStr_map()))

    {
        createRandomMap();
        ptr_gameWidget->newLog("Random Map Created");
    }

    ptr_gameWidget->gameWidCreateMatchfield(vec_hexGameGrid);
    countUnits();
    ptr_gameWidget->newLog("Einheiten gezählt");
    setFogOfWar();
    if(ptr_options->getBool_fogOfWar())
        ptr_gameWidget->newLog("Nebelmaschine angeschmissen");

    //Buttons Einfuegen
    createButtons();
    changeButtonPixmap();
    ptr_gameWidget->newLog("Buttons aufgenäht");

	//für KI
    if(init_options->getBool_ki())
    {
       ptr_playerTwo->setBoolKi(true);
       ptr_playerTwo->setPlayerName("KarlderKleine"); // wehe jemand ändert den npc namen
       ptr_gameKI = new KI(this, ptr_playerTwo, vec_hexGameGrid);
       ptr_gameWidget->newLog("KI: " + ptr_playerTwo->getPlayerName() + " wurde erstellt");
    }

    updateLabels();
    ptr_gameWidget->newLog("Label beschriftet");

}

Game::Game(QString filepath, GameWidget *gameWidegt)
    : ptr_hexSelectionCache(nullptr), ptr_gameWidget(gameWidegt), ptr_gameKI(nullptr)
{
    if(!readSaveGame(filepath))
    {
        createRandomMap();
    }else{
        ptr_gameWidget->newLog("Map geladen");
    }

    ptr_gameWidget->gameWidCreateMatchfield(vec_hexGameGrid);
    countUnits();
    ptr_gameWidget->newLog("Einheiten gezählt");
    setFogOfWar();
    if(ptr_options->getBool_fogOfWar())
        ptr_gameWidget->newLog("Nebelmaschine angeschmissen");

    //Buttons Einfuegen
    createButtons();
    ptr_gameWidget->newLog("Buttons aufgenäht");
    changeButtonPixmap();
    updateLabels();
    ptr_gameWidget->newLog("Label beschriftet");
}

Game::~Game()
{
    resetHexMatchfield();
    for(auto &it : vec_hexGameGrid)
    {
        for(auto &ut : it)
        {
            delete ut;
            ut = nullptr;
        }
        it.clear();
    }
    vec_hexGameGrid.clear();
    for(auto &it : vec_unitGrid)
    {
        for(auto &ut : it)
        {
            delete ut;
            ut = nullptr;
        }
        it.clear();
    }
    vec_unitGrid.clear();
    for(auto &it : ptr_gameWidget->getGameWidButtonScene()->items())
    {
        delete it;
        it = nullptr;
    }
    vec_buttonMenueBar.clear();
    delete ptr_playerOne;
    ptr_playerOne = nullptr;
    delete ptr_playerTwo;
    ptr_playerTwo = nullptr;
    delete ptr_gameKI;
    ptr_gameKI = nullptr;
}

void Game::saveGame()
{
    if(ptr_options->getStr_map() != "Random")
    {
        qDebug() << "SaveGame";
        QString qstring_pathToSaveFile = QFileDialog::getSaveFileName(ptr_gameWidget, tr("Save File"), "", tr("Data Text (*.txt);;All Files (*)"));

        if(qstring_pathToSaveFile.isEmpty())
        {
            qDebug() << "File Name Empty";
            return;
        }

        QFile file_saveFile(qstring_pathToSaveFile);

        if(!file_saveFile.open(QIODevice::WriteOnly))
        {
            qDebug() << " Could not open the file for writing";
            return;
        }

        QTextStream out(&file_saveFile);
        out.setCodec(QTextCodec::codecForName("UTF-8"));
        serialize(out);
    }else{
        qDebug() << "Random Spiele können nicht gespeichert werden";
    }
}

void Game::endGame()
{
    ptr_gameWidget->resetGameWidget();
    ptr_gameWidget->clearAllScenes();
    emit gameOver();
}

void Game::processSelection(HexagonMatchfield *selection)
{
    /*
     * Verarbeitet die Eingabe des Nutzers.
     * Zustad von Selection:
     *      INACTIVE: Der SelectionCache wird auf selection gesetzt. Zudem werden die Scenes in gameWidget entsprechend angepasst.
     *                  Der TargetCache wird zurückgesetzt und der state von selection wir auf ACTIVE gesetzt
     *      ACTIVE: Das Spielfeld wird zurückgesetzt und der SelectionCache auf nullptr
     *      TARGET: Wenn Round MOVE zurückliefert wird der Weg vom SelectionCache zur selection angezeigt
     *              Wenn Round ACTION zurückliefert wird die action Funktion der der ausgewählten Einheit ausgeführt
     *      PATH: Die ausgewählte Einheit wird auf die selection verlegt
     * */

    if(selection == nullptr)
    {
        qDebug() << "Fehler: selection ist ein Nullptr! In processSelection(HexagonMatchfield*)";
        return;
    }

    switch(selection->getState())
    {
    case INACTIVE:
        resetHexMatchfield();
        ptr_hexSelectionCache = selection;
        ptr_gameWidget->setInfoScene(ptr_hexSelectionCache->getPtr_hexMfieldDisplay());
        if(ptr_hexSelectionCache->getUnitStationed() != nullptr && ptr_hexSelectionCache->getUnitStationed()->getUnitPlayer() == ptr_playerActive)
        {
            ptr_hexSelectionCache->getUnitStationed()->resetBuildUnloadParameter();
            ptr_gameWidget->setOptScene(ptr_hexSelectionCache->getUnitStationed()->getVector_unitStorage());
        }
        //Angeklicktes auf AKTIVE setzten
        ptr_hexSelectionCache->setState(ACTIVE);
        break;

    case ACTIVE:
        resetHexMatchfield();

        /*Wenn vorher eine auswahl da war, welche ein transporter oder factory war, müssen die zurückgesetzt werden*/
        if(ptr_hexSelectionCache != nullptr && ptr_hexSelectionCache->getUnitStationed() != nullptr)
        {
            ptr_hexSelectionCache->getUnitStationed()->resetBuildUnloadParameter();
        }
        break;

    case TARGET:
        if(ptr_roundCurrent->getCurrentPhase() == MOVE)     //Move Phase
        {
            for(auto &it : set_hexTargetCache)        //Ziele auf zustand TARGET zurücksetzen
            {
                it->setState(TARGET);
            }
            showPath(selection);            //Weg von Ziel zu auswahl anzeigen

        }else if(ptr_roundCurrent->getCurrentPhase() == ACTION )    //Action Phase
        {
            if(ptr_hexSelectionCache->getUnitStationed()->action(selection, offset_distance(ptr_hexSelectionCache->getQpoint_gridPosition(), selection->getQpoint_gridPosition())))  //Wenn die Action geglückt ist
            {
                /*Prüfen ob eine neue Einheit auf dem Grid ist*/
                if(selection->getUnitStationed() != nullptr
                        && vec_unitGrid[selection->getQpoint_gridPosition().x()][selection->getQpoint_gridPosition().y()] == nullptr)
                {
                    vec_unitGrid[selection->getQpoint_gridPosition().x()][selection->getQpoint_gridPosition().y()] = selection->getUnitStationed(); // in das Grid einfügen
                    selection->getUnitStationed()->setPos(selection->pos());   //Position in der Scene setzen
                    selection->getUnitStationed()->setScale(ptr_gameWidget->getDouble_scaleFak()); // skalieren
                    ptr_gameWidget->getGameWidGameScene()->addItem(selection->getUnitStationed());    //in die Scene einfügen
                }
                ptr_gameWidget->setOptScene(ptr_hexSelectionCache->getUnitStationed()->getVector_unitStorage());

                ptr_gameWidget->setUnitsLabel(ptr_playerActive->getPlayerUnitNumber());    //Label updaten
                ptr_gameWidget->setEnergieLabel(ptr_playerActive->getCurrentEnergieStorage(), ptr_playerActive->getPlayerTotalEnergie());

                setFogOfWar();
            }
            resetTargetCache();
        }
        break;
    case PATH :
        moveUnitTo(selection);

        /*Neuen Selection Cache nach Bewegung*/
        ptr_hexSelectionCache = selection;
        ptr_hexSelectionCache->setState(ACTIVE);

        /*Darstellungen setzen*/
        ptr_gameWidget->setInfoScene(ptr_hexSelectionCache->getPtr_hexMfieldDisplay());
        ptr_gameWidget->setOptScene(ptr_hexSelectionCache->getUnitStationed()->getVector_unitStorage());

        resetTargetCache();
        setFogOfWar();
        break;
    }
    checkUnitGrid();
    countUnits();
    ptr_gameWidget->updateInfoOptScenes();
    ptr_gameWidget->updateMatchfieldScene();
    checkWinCondition();
}

void Game::Dijkstra(HexagonMatchfield* start, int factor)
{
    resetTargetCache();
    /*
     * Berechnung aller kürzesten Wege vom SelectionCache aus innerhalb der Reichweite
     * Setzt den TargetChache mit den berechneten Zielen und setzt deren state auf TARGET
     * */
    HexagonMatchfield* target = start;

    std::priority_queue<std::pair<HexagonMatchfield*, int>, std::vector<std::pair<HexagonMatchfield*, int>>, Compare> frontier;

    frontier.push(std::pair<HexagonMatchfield*, int>(target, 0));
    map_hexCameFrom[target] = target;
    map_hexCurrentCost[target] = 0;

    /*Solange es Elemente gibt, die in der Range liegen*/
    while(!frontier.empty())
    {
        /*Erstes Element aus der Schlange entfernen*/
        HexagonMatchfield* current = frontier.top().first;
        frontier.pop();

        /*Durchlaufen der Nachbarn des Elements*/
        vector<QPoint> neighbours;
        if(current->getQpoint_gridPosition().x() & 1)
        {
            neighbours = vec_qpointOddNeighbors;
        }else{
            neighbours = vec_qpointEvenNeighbors;
        }

        for(auto &it : neighbours)
        {
            int x = current->getQpoint_gridPosition().x() + it.x();
            int y = current->getQpoint_gridPosition().y() + it.y();

            /*Prüfen ob Nachbarn auf Spielfeld liegen*/
            if(x >= 0 && x < ptr_gameWidget->getSizeX() && y >= 0 && y < ptr_gameWidget->getSizeY()
                    && vec_hexGameGrid[x][y]->getState() != ACTIVE)
            {
                /*Speichern des zu betrachtenden Nachbarn*/
                HexagonMatchfield* neighbour = vec_hexGameGrid[x][y];

                /*Berechnen der neuen Kosten, bestehend aus den Kosten um auf das Aktuelle Feld zu kommen + die Kosten um zum  Nachbarn zu kommen*/
                if(target->getUnitStationed()->moveTo(neighbour) != -1)
                {

                        /*Wenn dieser noch nicht betrachtet wurde, kosten absurd hochlegen, damit diese auf jeden fall gesetzt werden*/
                        if(map_hexCurrentCost.find(neighbour) == map_hexCurrentCost.end())
                            map_hexCurrentCost[neighbour] = 999;

                        int new_cost = map_hexCurrentCost[current] + target->getUnitStationed()->moveTo(neighbour);

                        /*Wenn diese Kosten geringer als die Reichweite der Einheit und besser als die bisherigen Kosten sind, dann..*/
                        if(new_cost <= target->getUnitStationed()->getUnitCurrentMoveRange() * factor && new_cost < map_hexCurrentCost[neighbour])
                        {
                            map_hexCurrentCost[neighbour] = new_cost; //Kosten aktualisieren
                            map_hexCameFrom[neighbour] = current;     //Vorgänger auf das Aktuelle Feld setzem
                            frontier.push(std::pair<HexagonMatchfield*, int> (neighbour, map_hexCurrentCost[neighbour])); //Den Nachbarn der Queue hinzufügen

                            set_hexTargetCache.insert(neighbour);   //und in den Target Cache Stecken
                            neighbour->setState(TARGET);
                        }

                }
            }
        }
    }
}

bool Game::loadMapForNewGame(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "File nicht gefunden";
        return false;
    }

    QTextStream in(&file);
    QString typeHex;
    QString unitType;
    QString unitPath;
    int int_playerID;
    int int_isUnitSet;
    int boltanium;
    int dimX, dimY;

    in >> dimX;
    in >> dimY;
    ptr_gameWidget->setSizeX(dimX);
    ptr_gameWidget->setSizeY(dimY);

    //Hexagone einlesen
    for(int i = 0; i < dimX; i++)
    {
        std::vector<HexagonMatchfield*> vecHexagon;
        std::vector<Unit*> vecUnit;
        for(int j = 0; j < dimY; j++)
        {
            in >> typeHex;
            in >> boltanium;
            in >> int_isUnitSet;
            if(int_isUnitSet != 0)
            {
                in >> int_playerID;
                in >> unitType;
                in >> unitPath;
                Player* ptr_playerTemp = nullptr;
                if(int_playerID == 1)
                {
                    ptr_playerTemp = ptr_playerOne;
                }
                else{
                    ptr_playerTemp = ptr_playerTwo;
                }
                vecUnit.push_back(createUnitFromType(unitType, unitPath, ptr_playerTemp));
            }else{
                vecUnit.push_back(nullptr);
            }
            vecHexagon.push_back(new HexagonMatchfield(QPoint(i,j), typeHex, vecUnit[j]));
            vecHexagon[j]->setBoltaniumCurrent(boltanium);

            connect(vecHexagon[j],SIGNAL(SIGNAL_clicked(HexagonMatchfield*)),this,SLOT(processSelection(HexagonMatchfield*)));
            connect(vecHexagon[j],SIGNAL(SIGNAL_centerThis(HexagonMatchfield*)), ptr_gameWidget, SLOT(SLOT_gameWidCenterHex(HexagonMatchfield*)));
        }
        vec_unitGrid.push_back(vecUnit);
        vec_hexGameGrid.push_back(vecHexagon);
    }

    return true;
}

bool Game::loadMapFromSaveGame(QString filepath)
{

    QFile file(filepath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "File nicht gefunden";
        return false;
    }

    QTextStream in(&file);

    QString tmp;
    QString typeHex;
    int dimX, dimY;

    //Leere den das Spielfeld und befreie Speicher
    for(auto &it : vec_hexGameGrid)
    {
        for(auto &ut : it)
        {
            delete ut;
        }
        it.clear();
    }
    vec_hexGameGrid.clear();
    for(auto &it : vec_unitGrid)
    {
        for(auto &ut : it)
        {
            delete ut;
        }
        it.clear();
    }
    vec_unitGrid.clear();

    /*Einlesen der Dimensionen*/
    in >> dimX;
    in >> dimY;
    ptr_gameWidget->setSizeX(dimX);
    ptr_gameWidget->setSizeY(dimY);

    //Hexagone einlesen
    for(int i = 0; i < dimX; i++)
    {
        std::vector<HexagonMatchfield*> vecHexagon;
        for(int j = 0; j < dimY; j++)
        {
            in >> typeHex;
            vecHexagon.push_back(new HexagonMatchfield(QPoint(i,j), typeHex, nullptr));
            connect(vecHexagon[j],SIGNAL(SIGNAL_clicked(HexagonMatchfield*)),this,SLOT(processSelection(HexagonMatchfield*)));
            connect(vecHexagon[j],SIGNAL(SIGNAL_centerThis(HexagonMatchfield*)), ptr_gameWidget, SLOT(SLOT_gameWidCenterHex(HexagonMatchfield*)));
            tmp = in.readLine();
        }
        vec_hexGameGrid.push_back(vecHexagon);
    }


    //UnitGrid mit Nullptr initialisieren
    for(int i = 0; i < dimX; i++)
    {
        std::vector<Unit*> vecUnit;
        for(int j = 0; j < dimY; j++)
        {
            vecUnit.push_back(nullptr);
        }
        vec_unitGrid.push_back(vecUnit);
    }
    return true;
}

/*Move Button ausgewählt*/
void Game::buttonPressedMove()
{
    /*
     * Wenn der SLOT ausgelöst wird, wir
     * */
    if(ptr_roundCurrent->getCurrentPhase() == MOVE)     //Phase prüfen
    {
        /*Wenn ein Feld ausgewählt wurde auf dem eine Einheit steht, welche dem aktiven Spieler gehört*/
        if(ptr_hexSelectionCache != nullptr && ptr_hexSelectionCache->getUnitStationed() != nullptr
                && ptr_hexSelectionCache->getUnitStationed()->getUnitPlayer() == ptr_playerActive)
        {
            Dijkstra(ptr_hexSelectionCache);     //Berechnen aller möglichen Ziele
        }
    }
}

void Game::buttonPressedAction()
{
    if(ptr_roundCurrent->getCurrentPhase() == ACTION )
    {
        if(ptr_hexSelectionCache != nullptr && ptr_hexSelectionCache->getUnitStationed() != nullptr
                && ptr_hexSelectionCache->getUnitStationed()->getUnitPlayer() == ptr_playerActive
                && ptr_hexSelectionCache->getUnitStationed()->getUnitUsed() == false)
        {
            calculateTargets(ptr_hexSelectionCache, ptr_hexSelectionCache->getUnitStationed()->getActionRange());
        }
    }
}

void Game::buttonPressedMap()
{
        ptr_gameWidget->setEnableButtonScene(false);
        ptr_gameWidget->gameWidCreateMap(vec_hexGameGrid);
        ptr_gameWidget->getGameWidGameView()->setScene(ptr_gameWidget->getGameWidMapScene());
}

void Game::buttonPressedMenue()
{
    if(bool_menueView){
        ptr_gameWidget->getGameWidGameView()->setScene(ptr_gameWidget->getGameWidGameScene());
        ptr_gameWidget->setEnableButtonScene(true);
        bool_menueView = false;
    }else{
        ptr_gameWidget->getGameWidGameView()->setScene(ptr_gameWidget->getGameWidMenueScene());
        ptr_gameWidget->setEnableButtonScene(false);
        vec_buttonMenueBar[4]->setEnabled(true);
        bool_menueView = true;
    }
}

void Game::buttonPressedChangePhase()
{
    ptr_roundCurrent->changePhase();

    if(ptr_roundCurrent->getCurrentPhase() == MOVE)
    {
        ptr_playerActive = ptr_playerActive == ptr_playerOne ? ptr_playerTwo : ptr_playerOne;
        changeButtonPixmap();
        resetHexMatchfield();
        setFogOfWar();
        autoRepairUnits();
        checkWinCondition();
    }
    SLOT_checkStateOfButtons();
    resetTargetCache();

    if(ptr_hexSelectionCache != nullptr && ptr_hexSelectionCache->getUnitStationed() != nullptr)
    {
        ptr_hexSelectionCache->getUnitStationed()->resetBuildUnloadParameter();
    }
    resetUnits();
    updateLabels();
    ptr_gameWidget->repaintGameView();

	//Wenn activer Spieler KI ist dann autoplay
    if (ptr_playerActive->getBoolKi())
    {
        autoplayKi();
    }
}

void Game::buttonPressedZoomIn()
{
    if(ptr_gameWidget->getDouble_scaleFak() <= 3)
    {
        if(ptr_gameWidget->getDouble_scaleFak() >1)
            ptr_gameWidget->setDouble_scaleFak(ptr_gameWidget->getDouble_scaleFak() + 0.5);
        else
            ptr_gameWidget->setDouble_scaleFak(ptr_gameWidget->getDouble_scaleFak() + 0.1);
        for(auto & it : ptr_gameWidget->getGameWidGameScene()->items())
        {
            it->setScale(ptr_gameWidget->getDouble_scaleFak());
            ptr_gameWidget->getGameWidGameScene()->removeItem(it);
        }
        ptr_gameWidget->gameWidCreateMatchfield(vec_hexGameGrid);
    }
}

void Game::buttonPressedZoomOut()
{
    if(ptr_gameWidget->getDouble_scaleFak() >= 0.5)
    {
        if(ptr_gameWidget->getDouble_scaleFak() >1)
            ptr_gameWidget->setDouble_scaleFak(ptr_gameWidget->getDouble_scaleFak() - 0.5);
        else
            ptr_gameWidget->setDouble_scaleFak(ptr_gameWidget->getDouble_scaleFak() - 0.1);
        for(auto & it : ptr_gameWidget->getGameWidGameScene()->items())
        {
            it->setScale(ptr_gameWidget->getDouble_scaleFak());
            ptr_gameWidget->getGameWidGameScene()->removeItem(it);
        }
        ptr_gameWidget->gameWidCreateMatchfield(vec_hexGameGrid);
    }
}

void Game::SLOT_MenueButtonSelected(int menue)
{
    switch(menue)
    {
    case 0 : saveGame(); return;
    case 1 : endGame(); return;
    case 2 :
        ptr_gameWidget->getGameWidGameView()->setScene(ptr_gameWidget->getGameWidGameScene());
        ptr_gameWidget->setEnableButtonScene(true);
        return;
    }

}

void Game::SLOT_checkStateOfButtons()
{
    if(ptr_roundCurrent->getCurrentPhase() == MOVE)
    {
        vec_buttonMenueBar[0]->setEnabled(true); // Setze das der Movebutton nicht "geschwaertzt" werden soll
        vec_buttonMenueBar[1]->setEnabled(false);// Setze das der Actionbutton "geschwaertzt" werden soll
    }
    else
    {
        vec_buttonMenueBar[0]->setEnabled(false); // Setze das der Movebutton "geschwaertzt" werden soll
        vec_buttonMenueBar[1]->setEnabled(true); // Setze das der Actionbutton nicht "geschwaertzt" werden soll
    }
}

/*HilfsFunktionen Start#######################################################################*/
void Game::resetHexMatchfield()
{
    /*Zurücksetzen der Auswahl*/
    if(ptr_hexSelectionCache != nullptr)
    {
        ptr_hexSelectionCache->setState(INACTIVE);
        ptr_hexSelectionCache = nullptr;
    }
    resetTargetCache();
    ptr_gameWidget->clearInfoScene();
    ptr_gameWidget->clearOptionsScene();
}

void Game::resetTargetCache()
{
    for(auto &it : set_hexTargetCache)
    {
        it->setState(INACTIVE);
    }
    set_hexTargetCache.clear();
    map_hexCameFrom.clear();
    map_hexCurrentCost.clear();
}
void Game::moveUnitTo(HexagonMatchfield * target)
{
    Unit* unitToMove = ptr_hexSelectionCache->getUnitStationed();
    unitToMove->setUnitCurrentMoveRange(unitToMove->getUnitCurrentMoveRange() - map_hexCurrentCost[target]);

    if(target->getUnitStationed() != nullptr && target->getUnitStationed()->getUnitType().contains("TRANSPORTER"))
    {
        target->getUnitStationed()->addUnitToStorage(unitToMove); // Einheit in den Vektor der Transportereinheit verlegt
        ptr_gameWidget->getGameWidGameScene()->removeItem(unitToMove); // Einheit aus der Scene gelöscht
    }
    else
    {
        target->setUnitStationed(unitToMove);      //Einheit verlegen auf das Ziel
        vec_unitGrid[target->getQpoint_gridPosition().x()][target->getQpoint_gridPosition().y()] = unitToMove; //Einheit im Grid verlegt

        /*Animation*/
       if(!ptr_playerActive->getBoolKi())
       {
            vector<QPointF> path;
            for(auto& iterator = target; iterator != ptr_hexSelectionCache; iterator = map_hexCameFrom[iterator])
            {
                path.push_back(iterator->pos());
            }
            ptr_gameWidget->animateUnit(unitToMove, path);
       }
       else
            target->getUnitStationed()->setPos(target->pos());
    }

    vec_unitGrid[ptr_hexSelectionCache->getQpoint_gridPosition().x()][ptr_hexSelectionCache->getQpoint_gridPosition().y()] = nullptr; //Einheit aus dem UnitGrid löschen
    ptr_hexSelectionCache->setUnitStationed(nullptr);     //Einheit vom alten feld entfernen
    ptr_hexSelectionCache->setState(INACTIVE);     //Auswahl auf inactiv setzen
    ptr_hexSelectionCache = nullptr;
}
void Game::showNeighbors(HexagonMatchfield * center)
{
    if(center->getQpoint_gridPosition().x() & 1)
    {
        for(auto &it: vec_qpointOddNeighbors)
        {
            int x = center->getQpoint_gridPosition().x() + it.x();
            int y = center->getQpoint_gridPosition().y() + it.y();
            if(x >= 0 && x < ptr_gameWidget->getSizeX() && y >= 0 && ptr_gameWidget->getSizeY())
            {
                vec_hexGameGrid[x][y]->setState(TARGET);
                set_hexTargetCache.insert(vec_hexGameGrid[x][y]);
            }
        }
    }else{
        for(auto &it: vec_qpointEvenNeighbors)
        {
            int x = center->getQpoint_gridPosition().x() + it.x();
            int y = center->getQpoint_gridPosition().y() + it.y();
            {
                vec_hexGameGrid[x][y]->setState(TARGET);
                set_hexTargetCache.insert(vec_hexGameGrid[x][y]);
            }
        }
    }
}

void Game::calculateTargets(HexagonMatchfield * center, int range)
{
    resetTargetCache();
    std::queue<HexagonMatchfield*> frontier;
    frontier.push(center);

    while(!frontier.empty())    //Solange zulässige Ziele gefunden werdenn
    {
        HexagonMatchfield* current = frontier.front();
        frontier.pop();

        /*Durchlaufen der Nachbarn des Elements*/
        vector<QPoint> neighbours;
        if(current->getQpoint_gridPosition().x() & 1)
        {
            neighbours = vec_qpointOddNeighbors;
        }else{
            neighbours = vec_qpointEvenNeighbors;
        }

        for(auto &it : neighbours)    //Durchlaufen der Nachbarn
        {
            int x = current->getQpoint_gridPosition().x() + it.x();    //Koordinaten des neuen Ziels
            int y = current->getQpoint_gridPosition().y() + it.y();

            if(x >= 0 && x < ptr_gameWidget->getSizeX() && y >= 0 && y < ptr_gameWidget->getSizeY()         //Wenn das ziel auf dem Spielfeld liegt
                    && vec_hexGameGrid[x][y]->getState() != TARGET && vec_hexGameGrid[x][y]->getState() != ACTIVE)  //Wenn das Ziel nicht bereits als TARGET markiert wurde
            {
                HexagonMatchfield* neighbour = vec_hexGameGrid[x][y];         //Zwischenspeichern für lesbarkeite
                if(offset_distance(center->getQpoint_gridPosition(), neighbour->getQpoint_gridPosition()) <= range)          //Wenn das ziel in der Reichweite der Einheite liegt
                {
                    neighbour->setState(TARGET);
                    set_hexTargetCache.insert(neighbour);
                    frontier.push(neighbour);
                }
            }
        }
    }
}

void Game::setFogOfWar()
{
    if(ptr_options->getBool_fogOfWar() == true)
    {
        for(auto &iterator : vec_hexGameGrid)
        {
            for(auto &hex : iterator)
            {
                hex->setHexFogOfWar(true);
            }
        }
        for(auto &iterator : vec_hexGameGrid)
        {
            for(auto &hex : iterator)
            {
                if(hex->getUnitStationed() != nullptr && hex->getUnitStationed()->getUnitPlayer() == ptr_playerActive)
                {
                    calculateTargets(hex, hex->getUnitStationed()->getUnitView());
                    for(auto &it : set_hexTargetCache)
                    {
                        it->setHexFogOfWar(false);
                    }
                    hex->setHexFogOfWar(false);
                    resetTargetCache();
                }
            }
        }
        ptr_gameWidget->repaintGameView();
    }
}

void Game::showPath(HexagonMatchfield* target)
{
    for(auto &it = target; it != ptr_hexSelectionCache; it = map_hexCameFrom[it])
    {
        it->setState(PATH);
    }
}

void Game::checkUnitGrid()
{
    for(int x = 0; x < ptr_gameWidget->getSizeX(); x++)
    {
        for(int y = 0; y < ptr_gameWidget->getSizeY(); y++)
        {
            if(vec_unitGrid[x][y] != nullptr && vec_unitGrid[x][y]->checkUnitDestroyed())
            {
                vec_hexGameGrid[x][y]->setUnitStationed(nullptr);
                delete vec_unitGrid[x][y];
                vec_unitGrid[x][y] = nullptr;
            }
        }
    }
}

void Game::checkWinCondition()
{   //Prüfen ob ein Spieler verloren hat, also keine Einheiten oder kein HQ mehr hat
    if(ptr_playerOne->getPlayerUnitNumber() == 0 || ptr_playerOne->getHQDestroyed())
    {
        qDebug() << "Spieler Eins Verloren";
        ptr_gameWidget->showWinner(ptr_playerTwo);
        ptr_gameWidget->setEnableButtonScene(false);
    }else if(ptr_playerTwo->getPlayerUnitNumber() == 0 || ptr_playerTwo->getHQDestroyed())
    {
        qDebug() << "Spieler Zwei Verloren";
        ptr_gameWidget->showWinner(ptr_playerOne);
        ptr_gameWidget->setEnableButtonScene(false);
    }else if(ptr_roundCurrent->getCurrentRoundNumber() == ptr_roundCurrent->getMaxRoundNumber() + 10 && ptr_roundCurrent->getCurrentRoundNumber() != 10)
    {
        qDebug() << "Maximale Runde erreicht";
        if(ptr_playerOne->getPlayerUnitNumber() > ptr_playerTwo->getPlayerUnitNumber())
        {
            ptr_gameWidget->showWinner(ptr_playerOne);
            ptr_gameWidget->setEnableButtonScene(false);
        }
        else if(ptr_playerOne->getPlayerUnitNumber() > ptr_playerTwo->getPlayerUnitNumber())
        {
            ptr_gameWidget->showWinner(ptr_playerTwo);
            ptr_gameWidget->setEnableButtonScene(false);
        }
        else
        {
            ptr_gameWidget->showWinner();
            ptr_gameWidget->setEnableButtonScene(false);
        }
    }
}

void Game::resetUnits()
{
    for(auto &iteratorX : vec_unitGrid)    //Durchlaufen des Grids
    {
        for(auto &unit : iteratorX)
        {
            if(unit != nullptr)
            {
                unit->resetMovementRange();     //Untis Move range zurücksetzen
                unit->resetUnitUsed();
            }
        }
    }
}

void Game::countUnits()
{
    ptr_playerOne->setPlayerUnitNumber(0);
    ptr_playerTwo->setPlayerUnitNumber(0);

    for(auto &iterator : vec_unitGrid)
    {
        for(auto &unit : iterator)
        {
            if(unit != nullptr && unit->getUnitPlayer() != nullptr)
            {
                unit->getUnitPlayer()->increaseUnitNumber();

                if(unit->getUnitType().contains("TRANSPORTER"))
                {
                    for(auto &iteratorUnitStorage : unit->getVector_unitStorage())
                    {
                        iteratorUnitStorage->getUnitPlayer()->increaseUnitNumber();
                    }
                }
            }
        }
    }
}
/*Für Referenzen betrachte https://www.redblobgames.com/grids/hexagons/#distances , Kapitel: Distance*/
QVector3D Game::oddqToCube(QPoint oddqCoord)
{
    int x = oddqCoord.x();
    int z = oddqCoord.y() - (oddqCoord.x() - ((int)oddqCoord.x() & 1)) / 2;
    int y = (-1) * x - z;
    return QVector3D(x,y,z);
}

QPoint Game::cubeToOddq(QVector3D cubeCoord)
{
    int x = cubeCoord.x();
    int y = cubeCoord.z() + (cubeCoord.x() - ((int)cubeCoord.x() & 1)) / 2;
    return QPoint(x,y);
}

int Game::cube_distance(QVector3D a, QVector3D b)
{
    return (abs(a.x() - b.x()) + abs(a.y() - b.y()) + abs(a.z() - b.z())) / 2;
}

bool Game::readSaveGame(QString filepath)
{
    /*
     * Lade ein Spiel aus einer gespeicherten txt Datei mit dem Pfad filepath
     * Versionsnummer muss mit der Version des Ladens übereinstimmen
     * */
    QFile file(filepath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "File nicht gefunden";
        return false;
    }

    QTextStream in(&file);
    QString tmp;
    QString Version;
    in >> tmp;
    in >> tmp;
    in >> tmp;
    in >> Version;

    //Ueberpruefe Version
    if(Version != "V2.7")
    {
        qDebug() << "Falsche Version. Eingelesene Version: " << Version;
        return false;
    }

    tmp = in.readLine();

    //Ueberpruefe Format
    QString textType;
    textType = in.readLine();

    if(textType != "Save Game")
    {
        qDebug() << "Falsches Format. Eingelesen: " << textType;
        return false;
    }

    ptr_options = Options::unserialize(in);

    ptr_playerOne = Player::unserialize(in);
    ptr_playerTwo = Player::unserialize(in);

    qDebug() << "Player One: " << ptr_playerOne->getPlayerName();
    qDebug() << "Player Two: " << ptr_playerTwo->getPlayerName();
    qDebug() << "KI: " << (ptr_options->getBool_ki() == true ? "1" : "2");
    int playerActive;
    in >> playerActive;

    if(playerActive == 1)
    {
        ptr_playerActive = ptr_playerOne;
    }
    else
    {
        ptr_playerActive = ptr_playerTwo;
    }
    qDebug() << "Player Active : " << playerActive;

    ptr_roundCurrent = Round::unserialize(in);

    loadMapFromSaveGame(ptr_options->getStr_map());
    qDebug() << "Load from save game done";
    int posX, posY, boltanium, unitStationed;

    in >> tmp;
    while(tmp != "")
    {
       posX = tmp.toInt();
       in >> posY;
       in >> boltanium;
       in >> unitStationed;
        qDebug() << posX << " " << posY << " " << boltanium << " " << unitStationed;
       vec_hexGameGrid[posX][posY]->setBoltaniumCurrent(boltanium);

       if(unitStationed == 1)
       {
           vec_unitGrid[posX][posY] = readUnitFromStream(in);
           if(vec_unitGrid[posX][posY] != nullptr)
           {
               vec_hexGameGrid[posX][posY]->setUnitStationed(vec_unitGrid[posX][posY]);
           }

       }else{
           vec_unitGrid[posX][posY] = nullptr;
       }
       in >> tmp;
    }
    qDebug() << "Feld Eingelesen";
    if( ptr_options->getBool_ki() == true)
    {
        ptr_gameKI = new KI(this, ptr_playerTwo, vec_hexGameGrid);
    }
    qDebug() << "Ki erstellt";
    return true;
}

void Game::createRandomMap()
{

    //Für eine Zufallszahl
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    //Größe
    int sizeX = qrand() % 50 + 20;
    int sizeY = qrand() % 50 + 20;

    ptr_gameWidget->setSizeX(sizeX);
    ptr_gameWidget->setSizeY(sizeY);

    //HexagonMatchfield(QPoint (*,*), string type)
    //Mögliche Typen:
    //"waterDeep"           (Tiefes Meer)
    //"waterSeashore"       (Küste)
    //"forrest"             (Wald)
    //"grassland"           (Wiese)
    //"streetStraight"      (Straße Gerade)
    //"streetCurve"         (Straße mit Kurve)
    //"mountainTop"         (Bergspitze)
    //"mountainSide"        (Bergseite)
    for(auto &it : vec_hexGameGrid)
    {
        for(auto &ut : it)
        {
            delete ut;
        }
        it.clear();
    }
    vec_hexGameGrid.clear();

    for(auto &it : vec_unitGrid)
    {
        for(auto &ut : it)
        {
            delete ut;
        }
        it.clear();
    }
    vec_unitGrid.clear();

    for( int i = 0; i < sizeX; i++ )
    {
        vector<HexagonMatchfield*> vectorHex;
        for( int j = 0; j < sizeY; j++ )
        {
            if( i == 0 || i == sizeX-1 || j == 0 || j == sizeY-1 )
                vectorHex.push_back(new HexagonMatchfield(QPoint(i,j), "waterDeep", 0));
            else if( i == 1 || i == sizeX - 2 || j == 1 || j == sizeY - 2 )
                vectorHex.push_back(new HexagonMatchfield(QPoint(i,j), "waterSeashore", 0));
            else
            {
                int modulo = 10;
                if(vec_hexGameGrid[i-1][j]->getHexMatchfieldType() == "forrest")
                    modulo += 6;
                if(vec_hexGameGrid[i-1][j-1]->getHexMatchfieldType() == "forrest")
                    modulo += 6;
                if(vectorHex[j-1]->getHexMatchfieldType() == "forrest")
                    modulo += 6;
                int randomInt = qrand() % modulo;
                if(randomInt < 8)
                    if(randomInt < 1)
                        vectorHex.push_back(new HexagonMatchfield(QPoint(i,j), "mountainTop", 0));
                    else
                        vectorHex.push_back(new HexagonMatchfield(QPoint(i,j), "grassland", 0));
                else
                    vectorHex.push_back(new HexagonMatchfield(QPoint(i,j), "forrest", 0));
            }
            int randBoltanium = qrand() % 100;
            if(randBoltanium < 20)
            {
                int randBoltaniumAmount = (qrand() % 200) + 50;
                vectorHex[j]->setBoltaniumCurrent(randBoltaniumAmount);
            }
            connect(vectorHex[j],SIGNAL(SIGNAL_clicked(HexagonMatchfield*)),this,SLOT(processSelection(HexagonMatchfield*)));
            connect(vectorHex[j],SIGNAL(SIGNAL_centerThis(HexagonMatchfield*)), ptr_gameWidget, SLOT(SLOT_gameWidCenterHex(HexagonMatchfield*)));
        }
        vec_hexGameGrid.push_back(vectorHex);
    }

    qDebug() << "Bemerkung: Zufallsfeld erstellt (in Klasse Game). Nur für Testzwecke.";

    //Einheiten belegen

    int anzHQ = 0;

    for(int i = 0; i < sizeX; i++)
    {
        std::vector<Unit*> vectorUnit;
        for(int j = 0; j < sizeY; j++)
        {
                int randomInt = qrand() % 100;
                if(randomInt < 2)
                {
                    Unit* randomUnit = nullptr;
                    QString hexType = vec_hexGameGrid[i][j]->getHexMatchfieldType();
                    Player* randPlayer = (qrand() % 2) == 0 ? ptr_playerOne : ptr_playerTwo;

                    if(hexType != "waterDeep" && hexType != "waterSeashore")
                    {
                        int randomUnitType = qrand() % 9;

                        switch(randomUnitType)
                        {
                        case 0 : randomUnit = new AirUnit(":/dynamic/dynamicUnit/derbolten.txt", randPlayer); break;
                        case 1 : randomUnit = new TransporterGroundUnit(":/dynamic/dynamicUnit/kevarn.txt", randPlayer); break;
                        case 2 : randomUnit = new LightUnit(":/dynamic/dynamicUnit/ben.txt", randPlayer); break;
                        case 3 : randomUnit = new MediumUnit(":/dynamic/dynamicUnit/lucas.txt", randPlayer); break;
                        case 4 : randomUnit = new HeavyUnit(":/dynamic/dynamicUnit/mannuel.txt", randPlayer); break;
                        case 5 : randomUnit = new DepotUnit(":/static/staticUnit/depot.txt", randPlayer); break;
                        case 6 : randomUnit = new FactoryUnit(":/static/staticUnit/factory.txt", true, randPlayer); break;
                        case 7 :
                            if(anzHQ < 2)
                            {
                                randomUnit = new HeadquaterUnit(":/static/staticUnit/headquater.txt", anzHQ == 0 ? ptr_playerOne : ptr_playerTwo);
                                anzHQ++;
                            }
                            break;
                        case 8 : randomUnit = new BuildLightUnit(":/dynamic/dynamicUnit/scpmerlin.txt", true, randPlayer); break;
                        }
                    }else{
                        randomUnit = new WaterUnit(":/dynamic/dynamicUnit/msmiguel.txt", randPlayer);
                    }

                    vectorUnit.push_back(randomUnit);
                    vec_hexGameGrid[i][j]->setUnitStationed(vectorUnit[j]);

                }else{
                    vectorUnit.push_back(nullptr);
                }
        }
        vec_unitGrid.push_back(vectorUnit);
    }
}

void Game::createButtons()
{
    ButtonMove* movebutton = new ButtonMove(64,64);
    ButtonAction* actionbutton = new ButtonAction(64,64);
    ButtonChangePhase* changephasebutton = new ButtonChangePhase(64,64);
    ButtonMap* mapbutton = new ButtonMap(64,64);
    ButtonMenue* menuebutton = new ButtonMenue(64,64);
    ButtonZoomIn* zoomInbutton = new ButtonZoomIn(64,64);
    ButtonZoomOut* zoomOutbutton = new ButtonZoomOut(64,64);

    vec_buttonMenueBar.push_back(movebutton);
    vec_buttonMenueBar.push_back(actionbutton);
    vec_buttonMenueBar.push_back(changephasebutton);
    vec_buttonMenueBar.push_back(mapbutton);
    vec_buttonMenueBar.push_back(menuebutton);
    vec_buttonMenueBar.push_back(zoomInbutton);
    vec_buttonMenueBar.push_back(zoomOutbutton);
    connect(movebutton,SIGNAL(clicked()),this,SLOT(buttonPressedMove()));
    connect(actionbutton,SIGNAL(clicked()),this,SLOT(buttonPressedAction()));
    connect(changephasebutton,SIGNAL(clicked()),this,SLOT(buttonPressedChangePhase()));
    connect(mapbutton, SIGNAL(clicked()), this, SLOT(buttonPressedMap()));
    connect(menuebutton,SIGNAL(clicked()),this,SLOT(buttonPressedMenue()));
    connect(zoomInbutton, SIGNAL(clicked()), this, SLOT(buttonPressedZoomIn()));
    connect(zoomOutbutton,SIGNAL(clicked()),this,SLOT(buttonPressedZoomOut()));

    connect(ptr_gameWidget, SIGNAL(SIGNAL_MenueButtonPushed(int)), this, SLOT(SLOT_MenueButtonSelected(int)));
    connect(ptr_gameWidget, SIGNAL(SIGNAL_changeStateOfButtons()), this, SLOT(SLOT_checkStateOfButtons()));
    SLOT_checkStateOfButtons();

    ptr_gameWidget->gameWidCreateButtonBar(vec_buttonMenueBar);
}

Unit *Game::createUnitFromType(QString unitType, QString unitPath, Player * ptr_playerTemp)
{
    if(unitType == "FACTORYUNIT")
    {
        return new FactoryUnit(unitPath, true, ptr_playerTemp);
    }

    if( unitType == "HEADQUATERUNIT")
    {
        return new HeadquaterUnit(unitPath,ptr_playerTemp);
    }

    if( unitType == "DEPOTUNIT")
    {
        return new DepotUnit(unitPath,ptr_playerTemp);
    }

    if(unitType == "AIRUNIT")
    {
        return new AirUnit(unitPath, ptr_playerTemp);
    }

    if(unitType == "LIGHTUNIT")
    {
        return new LightUnit(unitPath, ptr_playerTemp);
    }

    if(unitType == "MEDIUMUNIT")
    {
        return new MediumUnit(unitPath, ptr_playerTemp);
    }

    if(unitType == "HEAVYUNIT")
    {
        return new HeavyUnit(unitPath, ptr_playerTemp);
    }

    if(unitType == "WATERUNIT")
    {
        return new WaterUnit(unitPath, ptr_playerTemp);
    }

    if(unitType.contains("TRANSPORTERGROUND"))
    {
        return new TransporterGroundUnit(unitPath, ptr_playerTemp);
    }

    if(unitType.contains("TRANSPORTERWATER"))
    {
        return new TransporterWaterUnit(unitPath, ptr_playerTemp);
    }

    if(unitType.contains("TRANSPORTERAIR"))
    {
        return new TransporterAirUnit(unitPath, ptr_playerTemp);
    }

    if(unitType == "BUILDERUNIT")
    {
        return new BuildLightUnit(unitPath, true,ptr_playerTemp);
    }

    return nullptr;
}

Unit *Game::readUnitFromStream(QTextStream &in)
{
    int unitPlayer, unitHP, unitMoveRange, unitUsed, unitLevel;
    QString unitType, unitPath;

    bool isDynamicUnit = true;
    bool isTransporterUnit = false;
    Unit* unitFromStream = nullptr;

    in >> unitType;
    in >> unitPath;
    in >> unitPlayer;
    in >> unitHP;
    in >> unitUsed;
    Player* ptr_playerTemp = nullptr;

    if(unitPlayer == 1)
    {
        ptr_playerTemp = ptr_playerOne;
    }else{
        ptr_playerTemp = ptr_playerTwo;
    }
    qDebug() << unitType << " " << unitPath;
    unitFromStream = createUnitFromType(unitType, unitPath, ptr_playerTemp);
    if(unitFromStream != nullptr)
    {
        qDebug() << unitType;
        unitFromStream->setUnitCurrentHP(unitHP);
        unitFromStream->setUnitUsed(unitUsed);

        if(unitType == "DEPOTUNIT" || unitType == "HEADQUATERUNIT" || unitType == "FACTORYUNIT")
        {
            qDebug() << "isDynamicUnit = false";
            isDynamicUnit = false;
        }else if(unitType.contains("TRANSPORTER"))
        {
            isTransporterUnit = true;
        }

        if(isDynamicUnit == true)
        {
            in >> unitLevel;
            in >> unitMoveRange;
            qDebug() << unitLevel << " " << unitMoveRange;
            unitFromStream->setUnitCurrentMoveRange(unitMoveRange);
            //unitFromStream->setUnitLevel(unitLevel);
        }

        if(isTransporterUnit == true)
        {
            loadInventory(in, unitFromStream);
        }
    }
    return unitFromStream;
}

void Game::loadInventory(QTextStream & in, Unit * containerUnit)
{
    int size = 0;
    in >> size;
    for(int i = 0; i < size; i++)
    {
        containerUnit->addUnitToStorage(readUnitFromStream(in));
    }
}

void Game::updateLabels()
{
    ptr_gameWidget->setPlayerLabel(ptr_playerActive->getPlayerName());
    ptr_gameWidget->setPhaseLabel(ptr_roundCurrent->getCurrentPhase() == MOVE ? "Move" : "Action");
    ptr_gameWidget->setUnitsLabel(ptr_playerActive->getPlayerUnitNumber());
    ptr_gameWidget->setEnergieLabel(ptr_playerActive->getCurrentEnergieStorage(), ptr_playerActive->getPlayerTotalEnergie());
    ptr_gameWidget->setRoundLabel(ptr_roundCurrent->getCurrentRoundNumber() / 10, ptr_roundCurrent->getMaxRoundNumber() / 10);
}

void Game::autoRepairUnits()
{
    for(auto &it : vec_unitGrid)
    {
        for(auto &ut: it)
        {
            if(ut != nullptr && ut->getUnitPlayer() == ptr_playerActive)
            {
                ut->autoRepair();
            }
        }
    }
}

int Game::offset_distance(QPoint a, QPoint b)
{
    QVector3D ac = oddqToCube(a);
    QVector3D bc = oddqToCube(b);
    return cube_distance(ac, bc);
}

void Game::serialize(QTextStream &out)
{
    out << "Battle Isle Clone V2.7\nSave Game\n";
    ptr_options->serialize(out);
    ptr_playerOne->serialize(out);
    ptr_playerTwo->serialize(out);
    out << ptr_playerActive->getPlayerID() << "\n";
    ptr_roundCurrent->serialize(out);
    for(auto &iteratorX : vec_hexGameGrid)
    {
        for(auto &hex : iteratorX)
        {
            hex->serialize(out);
        }
    }
}

void Game::changeButtonPixmap()
{
    if(ptr_playerActive == ptr_playerOne)
    {
        for(auto &it : vec_buttonMenueBar)
        {
            it->changePixmapPlayerOne();
        }
    }else{
        for(auto &it : vec_buttonMenueBar)
        {
            it->changePixmapPlayerTwo();
        }
    }
}


/*HilfsFunktionen Ende#######################################################################*/

bool Compare::operator()(std::pair<HexagonMatchfield*, int> a, std::pair<HexagonMatchfield*, int> b)
{
    return a.second < b.second;
}

//fuer ki
std::unordered_set<HexagonMatchfield*> Game::getTargetCache() const
{
    return set_hexTargetCache;
}

HexagonMatchfield* Game::getCamefrom_Hex(HexagonMatchfield* hex)
{
    return map_hexCameFrom[hex];
}

int Game::getCurrentCost_Int(HexagonMatchfield* hex)
{
    return map_hexCurrentCost[hex];
}

void Game::autoplayKi()
{
    resetTargetCache();
    if(ptr_roundCurrent->getCurrentPhase() == MOVE)
    {
        QElapsedTimer timer;
        timer.start();
        ptr_gameKI->autoPlayMove();
        ptr_gameWidget->newLog("KI Move: " + QString::number(timer.elapsed() / 1000.0) + "sec");
    }
    else
    {
        QElapsedTimer timer;
        timer.start();
        ptr_gameKI->autoPlayAction();
        ptr_gameWidget->newLog("KI Action: " + QString::number(timer.elapsed() / 1000.0) + "sec");
    }
}
