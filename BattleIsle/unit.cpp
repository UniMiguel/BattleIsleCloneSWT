/*
 * Author: Miguel
 * Version: 0.1
 * Datum 04.01.2018
 *
 * Author: Lucas
 * Version: 0.2
 * Datum 14.01.2018
 * Kommentar: Unit destruktor geschrieben, setUnitPlayer() bearbeitet -> erhöht den Unit Count bei dem Spieler der gesetzt wird
 * */
#include "unit.h"
#include <QFile>
#include <QPainter>
void Unit::setUnitCost(int value)
{
    int_unitCost = value;
}

Unit::Unit()
    : str_unitName(""), unitPlayer(nullptr), int_unitView(0), int_unitHP(0), int_unitCurrentHP(0),
      str_unitDetails(""), str_unitType(""), int_unitCost(0), int_unitStorageMax(0), bool_unitUsed(false),
      unitFile(""), int_unitAirAtt(0), int_unitGroundAtt(0), int_unitWaterAtt(0), int_unitEXP(0),
      int_unitMoveRange(0), int_unitCurrentMoveRange(0), ptr_UnitDisplay(new UnitDisplayInfo(this))
{
    setZValue(5);
}

Unit::~Unit()
{
    for(auto &it : vector_unitStorage)
    {
        delete it;
    }
    delete ptr_UnitDisplay;
}

bool Unit::checkUnitDestroyed()
{
    return int_unitCurrentHP <= 0 ? true : false;
}

int Unit::moveTo(HexagonMatchfield *)
{
    return -1;
}

void Unit::autoRepair()
{
    return;
}

void Unit::levelUpBonus()
{
    return;
}

void Unit::produceUnit(HexagonMatchfield*)
{
    return;
}

void Unit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(bool_unitUsed == true)
    {
        painter->setOpacity(.7);
    }
    QGraphicsPixmapItem::paint(painter,option,widget);

    QPen pen;
    QBrush brush;

    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::red);
    pen.setWidth(1);
    pen.setColor(Qt::red);

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(QRect(19,5,25,3));


    if(int_unitCurrentHP > 0)
    {
        brush.setColor(Qt::green);
        pen.setWidth(1);
        pen.setColor(Qt::green);

        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawRect(QRect(19,5,(double)int_unitCurrentHP / (double)int_unitHP * 25,3));
    }
}

void Unit::resetMovementRange()
{
    int_unitCurrentMoveRange = int_unitMoveRange;

}

void Unit::resetUnitUsed()
{
    bool_unitUsed = false;
}

void Unit::addUnitToStorage(Unit *)
{
    return;
}

void Unit::resetBuildUnloadParameter()
{
    return;
}


/*GET METHODEN #############################*/

int Unit::getUnitCurrentMoveRange() const
{
    return int_unitCurrentMoveRange;
}
int Unit::getUnitMoveRange() const
{
    return int_unitMoveRange;
}
int Unit::getUnitWaterAtt() const
{
    return int_unitWaterAtt;
}
int Unit::getActionRange() const
{
    return int_actionRange;
}
int Unit::getUnitGroundAtt() const
{
    return int_unitGroundAtt;
}
int Unit::getUnitAirAtt() const
{
    return int_unitAirAtt;
}
UnitDisplayInfo *Unit::getUnitDisplay() const
{
    return ptr_UnitDisplay;
}

std::vector<Unit *> Unit::getVector_unitStorage() const
{
    return vector_unitStorage;
}
QString Unit::getUnitName() const
{
    return str_unitName;
}

Player* Unit::getUnitPlayer() const
{
    return unitPlayer;
}

int Unit::getUnitEXP() const {
    return int_unitEXP;
}


int Unit::getUnitView() const
{
    return int_unitView;
}

int Unit::getUnitHP() const
{
    return int_unitHP;
}

int Unit::getUnitCurrentHP() const
{
    return int_unitCurrentHP;
}

QString Unit::getUnitDetails() const
{
    return str_unitDetails;
}

QString Unit::getUnitType() const
{
    return str_unitType;
}

int Unit::getUnitCost() const
{
    return int_unitCost;
}

bool Unit::getUnitUsed() const
{
    return bool_unitUsed;
}
/*SET METHODEN #############################*/
void Unit::setUnitCurrentMoveRange(int value)
{
    int_unitCurrentMoveRange = value;
}

void Unit::setUnitMoveRange(int value)
{
    int_unitMoveRange = value;
}
void Unit::setUnitWaterAtt(int value)
{
    int_unitWaterAtt = value;
}

void Unit::setUnitGroundAtt(int value)
{
    int_unitGroundAtt = value;
}

void Unit::setUnitAirAtt(int value)
{
    int_unitAirAtt = value;
}


void Unit::setVector_unitStorage(const std::vector<Unit *> &value)
{
    vector_unitStorage = value;
}

void Unit::setUnitEXP(int value){
	int_unitEXP = value;
}

void Unit::setUnitName(QString name)
{
	str_unitName = name;
	return;
}

void Unit::setUnitPlayer(Player* player)
{
    unitPlayer = player;
}

void Unit::setUnitHP(int hp)
{
	int_unitHP = hp;
	return;
}

void Unit::setUnitCurrentHP(int hp)
{
	int_unitCurrentHP = hp;
	return;
}

void Unit::setUnitUsed(bool used)
{
	bool_unitUsed = used;
	return;
}

void Unit::setUnitToBuild(QString)
{
    return;
}
