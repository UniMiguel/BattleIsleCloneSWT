/*
 * Author: Lucas
 * Version: 0.1
 * Datum 01.12.2017
 *
 * Author: Manuel
 * Version: 0.2
 * Datum 11.12.2017
 * Kommentar: Klassen mit Operationen hinzugefuegt
 *
 * Author: Manuel
 * Version: 0.3
 * Datum: 04.01.2018
 * Kommentar: Geändert nach dem neuen UML Diagramm
 *
 * Author: Manuel
 * Version: 0.4
 * Datum: 08.01.2018
 *
 * Author: Lucas, Manuel
 * Version: 0.5
 * Datum: 13.01.2018
 * */
#ifndef HEXAGONMATCHFIELD_H
#define HEXAGONMATCHFIELD_H

#include "enumerations.h"
#include "hexagonbase.h"
#include "unit.h"
#include "gamewidget.h"

#include <QString>

class Unit;
class Game;
class HexagonDisplayInfo;

class HexagonMatchfield : public HexagonBase
{
    Q_OBJECT
protected:
    /* Zum setZValue:
     * -1 : Units wenn in Fog of War
     * 0: alle inaktiven Hexagons
     * 1: Targets
     * 2: Active
     * 3:
     * 4:
     * 5: Units
     */
    bool bool_hexFogOfWar;
    Unit* ptr_unitStationed;
    MATCHFIELDSTATE state;
    HexagonDisplayInfo* ptr_hexDisplay;
    QString qstring_hexType;
    //Mögliche Typen:
    //"waterDeep"           (Tiefes Meer)
    //"waterSeashore"       (Küste)
    //"forrest"             (Wald)
    //"grassland"           (Wiese)
    //"streetStraight"      (Straße Gerade)
    //"streetCurve"         (Straße mit Kurve)
    //"streetCrossing"      (Kreuzung von Straßen)
    //"mountainTop"         (Bergspitze)
    //"mountainSide"        (Bergseite)
    int int_boltaniumCurrent;
    QColor qcolor_hexColor;
    QPixmap qpixmap_boltanium;

public:
    HexagonMatchfield(QPoint qpoint_gridPosition, QString type, Unit* stationedUnit = nullptr);
    ~HexagonMatchfield();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void serialize(QTextStream&);


    //get und set Methoden
    QString getHexMatchfieldType() {return qstring_hexType;}
    MATCHFIELDSTATE getState() const;
    HexagonDisplayInfo *getPtr_hexMfieldDisplay() const;
    void setState(const MATCHFIELDSTATE &value);

    Unit *getUnitStationed() const;
    void setUnitStationed(Unit *value);
    
    int getBoltaniumCurrent() const;
    void setBoltaniumCurrent(int value);

    QColor getQcolor_HexColor() const;

    bool getHexFogOfWar() const;
    void setHexFogOfWar(bool value);

public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent* event = nullptr);

signals:
    void SIGNAL_clicked(HexagonMatchfield*);
    void SIGNAL_centerThis(HexagonMatchfield*);
};

#endif // HEXAGONMATCHFIELD_H
