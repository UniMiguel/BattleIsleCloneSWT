/*
 * Author: Miguel
 * Version: 0.2
 * Datum 03.01.2018
 */
/*
 * Author: Lucas
 * Version: 0.3
 * Datum 14.01.2018
 * Kommentar: Chechunitdestroyed von HQ ausgeprägt
 */


#ifndef STATICUNIT_HPP
#define STATICUNIT_HPP

#include "hexagonmatchfield.h"
#include "player.h"
#include "unit.h"
#include "dynamicunit.h"

#include <map>
#include <QTextStream>
#include <QString> 

class Player;
class HexagonMatchfield;

class StaticUnit : public Unit
{
protected:
	int int_EnergieStorage;

public:
    StaticUnit(QString, Player* = nullptr);
    void serialize(QTextStream &);
	int getEnergieStorage() const;	
	int moveTo(HexagonMatchfield*);
};

class DepotUnit : public StaticUnit
{
public:
    DepotUnit(QString, Player* = nullptr);
    ~DepotUnit();

	bool action(HexagonMatchfield*, const int = 0);
	void repairUnit(Unit*);
	Unit* createUnit();
};

class FactoryUnit : public StaticUnit
{
    Q_OBJECT
protected:
    QString qstring_unitToBuild;
    std::map<QString, Unit*> map_production;

public:
    FactoryUnit(QString, bool ,Player* = nullptr);
    ~FactoryUnit();

	QString getUnitToBuild() const;
	void setUnitToBuild(const QString);

	bool action(HexagonMatchfield*, const int = 0);
	void produceUnit(HexagonMatchfield*);
	Unit* createUnit();

    void resetBuildUnloadParameter();
    //für die KI

private slots:
    void SLOT_setUnitToBuild(Unit* unit);
};

class HeadquaterUnit : public FactoryUnit
{
public:
    HeadquaterUnit(QString, Player* = nullptr);

    virtual bool checkUnitDestroyed();
    Unit* createUnit();
};


#endif
