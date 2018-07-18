/*
 * Author: Miguel
 * Version: 0.1
 * Datum 03.01.2018
 *
 * Author: Lucas
 * Version: 0.2
 * Datum 14.01.2018
 * Kommentar: Unit destruktor geschrieben, setUnitPlayer() bearbeitet -> erhöht den Unit Count bei dem Spieler der gesetzt wird
 * */

#ifndef UNIT_HPP
#define UNIT_HPP

#include "hexagonmatchfield.h"
#include "player.h"
#include "gamewidget.h"

#include <map>
#include <vector>
#include <QString> 
#include <QTextStream>

class HexagonMatchfield;
class UnitDisplayInfo;
class Player;

class Unit : public QObject, public QGraphicsPixmapItem
{
protected:
	QString str_unitName;
    Player* unitPlayer;
	int int_unitView;
	int int_unitHP;
	int int_unitCurrentHP;
	QString str_unitDetails;
	QString str_unitType;
	int int_unitCost;

    std::vector<Unit*> vector_unitStorage;
	int int_unitStorageMax;
	bool bool_unitUsed;
	QString unitFile;
	
     //von dynamic Unit verschobene Variablen [Auf Grund von Zurückangreifen]
    int int_unitAirAtt;
    int int_unitGroundAtt;
    int int_unitWaterAtt;
    int int_unitEXP;

    int int_unitMoveRange;
    int int_unitCurrentMoveRange;
    int int_actionRange;

    UnitDisplayInfo* ptr_UnitDisplay;

public:
	Unit();
    ~Unit();

    virtual Unit* createUnit() = 0;
    virtual bool checkUnitDestroyed();
    virtual bool action(HexagonMatchfield*, const int = 0) = 0;
    virtual void serialize(QTextStream&) = 0;
    virtual int moveTo(HexagonMatchfield*);
    virtual void autoRepair();
    virtual void levelUpBonus();
    virtual void produceUnit(HexagonMatchfield*);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /*Hilfsfunktionen*/
    virtual void resetMovementRange();
    virtual void resetUnitUsed();
    virtual void addUnitToStorage(Unit* unit);
    virtual void resetBuildUnloadParameter();
    /*Hilfsfunktionen Ende*/
    // Hilfsfunktion für KI
    virtual void setUnitToBuild(QString);

	QString getUnitName() const;
	Player* getUnitPlayer() const;
	int getUnitView() const;
	int getUnitHP() const;
	int getUnitCurrentHP() const;
	QString getUnitDetails() const;
	QString getUnitType() const;
	int getUnitCost() const;
	bool getUnitUsed() const;
    UnitDisplayInfo *getUnitDisplay() const;
    int getUnitEXP() const;
    int getUnitAirAtt() const;
    int getUnitGroundAtt() const;
    int getUnitWaterAtt() const;
    int getUnitMoveRange() const;
    int getUnitCurrentMoveRange() const;
    std::vector<Unit *> getVector_unitStorage() const;
    int getActionRange() const;

    void setUnitCurrentMoveRange(int value);
    void setUnitName(QString);
	void setUnitPlayer(Player*);
	void setUnitHP(int);
	void setUnitCurrentHP(int);
	void setUnitUsed(bool);
    void setUnitMoveRange(int value);
    void setUnitWaterAtt(int value);
    void setUnitGroundAtt(int value);
    void setUnitAirAtt(int value);
    void setUnitEXP(const int);
    void setVector_unitStorage(const std::vector<Unit *> &value);
    void setUnitCost(int value);
};


#endif
