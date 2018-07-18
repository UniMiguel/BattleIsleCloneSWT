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
 * Author: Lucas, Manuel
 * Version: 0.4
 * Datum: 13.01.2018
 *
 * Author: Lucas
 * Version: 0.5
 * Datum: 14.01.2018
 * Kommentar: Label für den zustand des spiels eingefügt: player & playerUnitNumber
 * */
#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H


#include "hexagonmatchfield.h"
#include "unit.h"
#include "button.h"
#include "player.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <vector>
#include <QPushButton>
#include <QPointer>

class Unit;
class Player;

namespace Ui {
class GameWidget;
}

class Game; //Vorwärtsdeklaration
class HexagonMatchfield;
class HexagonDisplayInfo;
class MapPixel;

class GameWidget : public QWidget
{
    Q_OBJECT
private:
    Ui::GameWidget *ui;
    QGraphicsScene *gameWidGameScene;
    QGraphicsScene *gameWidInfoScene;
    QGraphicsScene *gameWidOptionsScene;
    QGraphicsScene *gameWidButtonScene;
    QGraphicsScene *gameWidMenueScene;
    QGraphicsScene *gameWidMapScene;

    QPointer<QGraphicsTextItem> textItem_currentPlayer;
    QPointer<QGraphicsTextItem> textItem_currentEnergie;
    QPointer<QGraphicsTextItem> textItem_currentUnits;
    QPointer<QGraphicsTextItem> textItem_currentPhase;
    QPointer<QGraphicsTextItem> textItem_currentRound;

    QPushButton* qbuttonSaveGame;
    QPushButton* qbuttonEndGame;
    QPushButton* qbuttonLoadGame;
    QPushButton* qbuttonResume;

    std::vector<std::vector< QPointer<MapPixel> >> vectorVector_gameWidMiniMap;

    int sizeX;
    int sizeY;

    double double_scaleFak;

public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();
    void resizeEvent(QResizeEvent*, int mainHeight, int mainWdith);
    void gameWidCreateMatchfield(std::vector<std::vector<HexagonMatchfield*>> &hexagonGrid);
    void gameWidCreateButtonBar(std::vector<Button *> buttonVector);
    void gameWidCreateMap(std::vector<std::vector<HexagonMatchfield*>> &hexagonGrid); //Erstellt die Map Uebersicht
    void clearButtonScene();
    void clearInfoScene();
    void clearOptionsScene();
    void clearGameScene();
    void clearAllScenes(); //Loescht alle Scenen
    void resetGameWidget();
    void setEnableButtonScene(bool);
    void updateInfoOptScenes();
    void updateMatchfieldScene();
    void repaintGameView();
    void showWinner(Player* winner = 0);
    void animateUnit(Unit*, std::vector<QPointF>);
    void newLog(QString);

    //get und set Methoden
    void setInfoScene(HexagonDisplayInfo *info);
    void setOptScene(std::vector<Unit*>);
    void setPlayerLabel(QString);
    void setPhaseLabel(QString);
    void setUnitsLabel(int value);
    void setEnergieLabel(int current, int max);
    void setRoundLabel(int current, int max);
    void setSizeX(int value);
    void setSizeY(int value);
    void setDouble_scaleFak(double value);

    int getSizeX() const;
    int getSizeY() const;
    QGraphicsScene *getGameWidInfoScene() const;
    QGraphicsScene *getGameWidOptionsScene() const;
    QGraphicsScene *getGameWidGameScene() const;
    QGraphicsScene *getGameWidButtonScene() const;
    QGraphicsScene *getGameWidMenueScene() const;
    QGraphicsView* getGameWidGameView() const;
    QGraphicsScene *getGameWidMapScene() const;
    double getDouble_scaleFak() const;


private:
    void gameWidCreateMenueScene();

private slots:
    void SLOT_qbuttonSaveGame_clicked();
    void SLOT_qbuttonEndGame_clicked();
    void SLOT_qbuttonResume_clicked();
    void SLOT_gameWidCenterHex(HexagonMatchfield* hex); //Setzt ein Hexagon in die Mitte der View
    void SLOT_gameWidDestroyMap(); //Loescht die Minimap wieder ordnungsgemaess

signals:
    void SIGNAL_gameWidChangeIndexFromStack(int);
    void SIGNAL_MenueButtonPushed(int);
    void SIGNAL_changeStateOfButtons(); // Verbunden mit SLOT_checkStateOfButtons in der Klasse Game
};

//Klasse um die Hexagons in der Infobox anzeigen zu lassen
class HexagonDisplayInfo : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

private:
    HexagonMatchfield* hexToDisplay;
    QString qStringHexDispHexType;
    QString qStringHexDisplayHexBoltanium;
    QString qStringUnitDispUnitName;
    QString qStringUnitDispUnitType;
    QString qStringUnitDispUnitLife;
    QString qStringUnitDispMovement;
    QString qStringUnitDispActionRange;
    QString qStringUnitDispUnitAttack;
public:
    HexagonDisplayInfo(HexagonMatchfield* ptr_hexMfield);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void updateText();
};

//Klasse um die Hexagons in der Infobox anzeigen zu lassen
class UnitDisplayInfo : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

private:
    Unit* ptr_UnitToDisplay;
    QString qStringUnitDispUnitName;
    QString qStringUnitDispUnitType;
    QString qStringUnitDispUnitLife;
    QString qStringUnitDispMovement;
    QString qStringUnitDispActionRange;
    QString qStringUnitDispUnitAttack;
    QString qStringUnitDispUnitCost;
    QColor color;

public:
    UnitDisplayInfo(Unit* ptr_Unit);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void updateText();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void setColor(const QColor &value);

signals:
    void unitDispl_clicked(Unit* unit);
};


class MapPixel : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
private:
    HexagonMatchfield* ptr_mapPixHexaon;
    QPoint qpoint_mapPixPosition;
public:
    MapPixel(int x, int y, HexagonMatchfield* hex);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    QRectF boundingRect();

signals:
    void SIGNAL_mapPixelClicked();
};
#endif // GAMEWIDGET_H
