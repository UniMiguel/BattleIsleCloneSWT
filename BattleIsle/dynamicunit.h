/*
 * Author: Arne
 * Version: 0.2
 * Kommentar: Wer noch irgendwo Static statt Dynamic findet bekommt n Eis von mir, lg Arne
 *
 * Author: Lucas
 * Version: 0.3
 * Datum: 12.01.17
 * Kommentar: Fehlerbehebung
 *
 * Author: Lucas, Manuel
 * Version: 0.4
 * Datum: 13.01.2018
 */
#ifndef DYNAMICUNIT_HPP
#define DYNAMICUNIT_HPP

#include "hexagonmatchfield.h"
#include "enumerations.h"
#include "unit.h"

class DynamicUnit : public Unit
{
protected:
      int int_unitAutoRep;
      int int_unitLevel;


public:
    DynamicUnit(QString, Player*);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void serialize(QTextStream&);

    void autoRepair();
    void levelUpBonus();
    void resetUnit();

    int getUnitAutoRep() const;
    int getUnitLevel() const;

    void setUnitAutoRep(const int);
    void setUnitLevel(const int);
};

// Air

class AirUnit : public DynamicUnit
{
public:
    AirUnit(QString, Player*  = nullptr);

	int moveTo(HexagonMatchfield* );
	bool action(HexagonMatchfield*, const int = 0 );
    virtual AirUnit* createUnit();
};

// Transporter

class TransporterUnit : public DynamicUnit
{
    Q_OBJECT
protected:
	int int_transporterUnitCapacity;
	int int_transporterUnitCurrentCapacity;
    Unit* ptr_unitToUnload;
public:
	TransporterUnit(QString, Player*);

    virtual bool action(HexagonMatchfield *, const int = 0);
    void unload(HexagonMatchfield *);
    void farmBoltanium(HexagonMatchfield *);
    void addUnitToStorage(Unit* );
    void serialize(QTextStream &);

    void resetBuildUnloadParameter();

    int getTransporterUnitCapacity() const;
    int getTransporterUnitCurrentCapacity() const;
    void setTransporterUnitCurrentCapacity(const int);

public slots:
    void SLOT_setUnitToUnload(Unit*);
};

//TransporterAir

class TransporterAirUnit : public TransporterUnit
{
public:
    TransporterAirUnit(QString, Player*);

  int moveTo(HexagonMatchfield* hex_target);
  virtual TransporterAirUnit* createUnit();
};

//TransporterGround

class TransporterGroundUnit : public TransporterUnit
{
public:
    TransporterGroundUnit(QString, Player*);

  int moveTo(HexagonMatchfield* );
  virtual TransporterGroundUnit* createUnit();
};

// TransporterWater

class TransporterWaterUnit : public TransporterUnit
{
public:
    TransporterWaterUnit(QString, Player*);

  int moveTo(HexagonMatchfield* );
  virtual TransporterWaterUnit* createUnit();
};

//Ground

class GroundUnit : public DynamicUnit
{
public:
    GroundUnit(QString, Player*  = nullptr);

	 bool action(HexagonMatchfield* , const int = 0 );     
};

// Light

class LightUnit : public GroundUnit
{
public:
    LightUnit(QString, Player*  = nullptr);

	int moveTo(HexagonMatchfield* );
    virtual LightUnit* createUnit();
};

// BuilderLight

class BuildLightUnit : public LightUnit
{
    Q_OBJECT
    QString qstring_unitToBuild;
    std::map<QString, Unit*> production;
public:
    BuildLightUnit(QString,bool, Player*  = nullptr);
    ~BuildLightUnit();

    bool action(HexagonMatchfield* , const int = 0 );
    void produceUnit(HexagonMatchfield *hex_target);
    virtual BuildLightUnit* createUnit();
    void resetBuildUnloadParameter();
    //für die KI
    virtual void setUnitToBuild(QString);

private slots:
    void SLOT_setUnitToBuild(Unit* unit);
};

// Medium

class MediumUnit : public GroundUnit
{
public:
    MediumUnit(QString, Player*  = nullptr);

	int moveTo(HexagonMatchfield* );
    virtual MediumUnit* createUnit();
};

// Heavy

class HeavyUnit : public GroundUnit
{
public:
    HeavyUnit(QString, Player*  = nullptr);

    int moveTo(HexagonMatchfield* );
    virtual HeavyUnit* createUnit();
};


// Water

class WaterUnit : public DynamicUnit
{
public:
    WaterUnit(QString, Player*  = nullptr);

	int moveTo(HexagonMatchfield* );
    bool action(HexagonMatchfield* , const int = 0 );
    virtual WaterUnit* createUnit();
};

#endif
