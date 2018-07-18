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

#include "staticunit.h"
#include <QFile>
#include <QPainter>
//StaticUnit

StaticUnit::StaticUnit(QString filepath, Player* player)
    : Unit()
{
    QFile file(filepath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Warnung: File nicht gefunden";
        return;
    }
    QTextStream in(&file);
    str_unitName = in.readLine();
	in >> int_unitView;
    in >> int_actionRange;
	in >> int_unitHP;
	in >> int_unitStorageMax;
    in >> int_EnergieStorage;
    in >> str_unitType;

    QString buffer; //Aufgrund des Zeilenumbruchs
    buffer = in.readLine();

    str_unitDetails = in.readLine();

    QString pixmapPathPlayerNeutral = in.readLine();
    QString pixmapPathPlayerOne = in.readLine();
    QString pixmapPathPlayerTwo = in.readLine();

    QPixmap pix;
    if(player == nullptr)
    {
        pix.load(pixmapPathPlayerNeutral);
    }else if(player->getPlayerID() == 1)
    {
        pix.load(pixmapPathPlayerOne);
    }else if(player->getPlayerID() == 2)
    {
        pix.load(pixmapPathPlayerTwo);
    }

    setPixmap(pix.scaled(64,64));

    int_unitCurrentHP = int_unitHP;
    unitFile = filepath;
    setUnitPlayer(player);

    if(int_unitView < int_unitMoveRange)
    {
        int_unitView = int_unitMoveRange;
    }
}

void StaticUnit::serialize(QTextStream &out)
{
    out << str_unitType << " " << unitFile << " " << unitPlayer->getPlayerID() << " "
        << int_unitCurrentHP << " " << bool_unitUsed << "\n";
}

int StaticUnit::getEnergieStorage() const
{
	return int_EnergieStorage;
}

int StaticUnit::moveTo(HexagonMatchfield* )
{
    return 1;
}

// HQ

HeadquaterUnit::HeadquaterUnit(QString filepath, Player* player)
    : FactoryUnit(filepath, true, player)
{
}

bool HeadquaterUnit::checkUnitDestroyed()
{
    if(int_unitCurrentHP <= 0)
    {
        unitPlayer->setHQDestroyed(true);
        return true;
    }
    return false;
}

Unit* HeadquaterUnit::createUnit()
{
    return new HeadquaterUnit(unitFile, unitPlayer);
}



// Depot

DepotUnit::DepotUnit(QString filepath, Player* player)
    : StaticUnit(filepath, player)
{
    int_unitCost = 150;
    if(player != nullptr)
    {
        player->setPlayerTotalEnergie(player->getPlayerTotalEnergie() + int_EnergieStorage);
    }
}

DepotUnit::~DepotUnit()
{
    if(unitPlayer != nullptr)
        unitPlayer->setPlayerTotalEnergie(unitPlayer->getPlayerTotalEnergie() - int_EnergieStorage);
}

bool DepotUnit::action(HexagonMatchfield* hexTarget, const int)
{
    if(bool_unitUsed == true)
    {
        return false;
    }
    if(hexTarget->getUnitStationed() != nullptr && hexTarget->getUnitStationed()->getUnitPlayer() == unitPlayer)
    {
        repairUnit(hexTarget->getUnitStationed());
        bool_unitUsed = true;
        return true;
    }
    return false;
}

void DepotUnit::repairUnit(Unit* unitTarget)
{	
    if(unitTarget->getUnitCurrentHP() < unitTarget->getUnitHP() && unitPlayer->getCurrentEnergieStorage() - (unitTarget->getUnitHP() - unitTarget->getUnitCurrentHP()) > 0)
	{
        unitPlayer->setCurrentEnergieStorage(unitPlayer->getCurrentEnergieStorage() - (unitTarget->getUnitHP() - unitTarget->getUnitCurrentHP()));
		unitTarget->setUnitCurrentHP(unitTarget->getUnitHP());
		bool_unitUsed = true;
	}
	return;
}

Unit* DepotUnit::createUnit()
{
    return new DepotUnit(unitFile, unitPlayer);
}

// Factory

FactoryUnit::FactoryUnit(QString filepath, bool bool_loadInventory, Player* player)
    : StaticUnit(filepath, player), qstring_unitToBuild("")
{
    int_unitCost = 200;
    if(bool_loadInventory == true)
    {
        map_production["B.E.N"] = new LightUnit(":/dynamic/dynamicUnit/ben.txt", unitPlayer);
        map_production["FAV-Buster"] = new LightUnit(":/dynamic/dynamicUnit/favbuster.txt", unitPlayer);
        map_production["R-1 Demon"] = new LightUnit(":/dynamic/dynamicUnit/r1demon.txt", unitPlayer);

        map_production["Lucas"] = new MediumUnit(":/dynamic/dynamicUnit/lucas.txt", unitPlayer);
        map_production["T-3 Scorpion"] = new MediumUnit(":/dynamic/dynamicUnit/t3scorpion.txt", unitPlayer);
        map_production["T-4 Gladiator"] = new MediumUnit(":/dynamic/dynamicUnit/t4gladiator.txt", unitPlayer);

        map_production["Mann u. El"] = new HeavyUnit(":/dynamic/dynamicUnit/mannuel.txt", unitPlayer);
        map_production["AD9-Sphinx"] = new HeavyUnit(":/dynamic/dynamicUnit/ad9sphinx.txt", unitPlayer);
        map_production["TLAV Invader"] = new HeavyUnit(":/dynamic/dynamicUnit/tlavinvader.txt", unitPlayer);

        map_production["Kevarn"] = new TransporterGroundUnit(":/dynamic/dynamicUnit/kevarn.txt", unitPlayer);
        map_production["G-2 Giant"] = new TransporterAirUnit(":/dynamic/dynamicUnit/g2giant.txt", unitPlayer);
        map_production["SC-P Merlin"] = new BuildLightUnit(":/dynamic/dynamicUnit/scpmerlin.txt", false, unitPlayer);

        map_production["Der Bolten"] = new AirUnit(":/dynamic/dynamicUnit/derbolten.txt", unitPlayer);
        map_production["XA-7 Raven"] = new AirUnit(":/dynamic/dynamicUnit/xa7raven.txt", unitPlayer);
        map_production["XF-7 Mosquito"] = new AirUnit(":/dynamic/dynamicUnit/xf7mosquito.txt", unitPlayer);

        map_production["M.S Miguel"] = new WaterUnit(":/dynamic/dynamicUnit/msmiguel.txt", unitPlayer);
        map_production["Z-1 Pegasus"] = new TransporterWaterUnit(":/dynamic/dynamicUnit/z1pegasus.txt", unitPlayer);
        map_production["CV-Amazon"] = new WaterUnit(":/dynamic/dynamicUnit/cvamazon.txt", unitPlayer);
        map_production["MB-A Buccaneer"] = new WaterUnit(":/dynamic/dynamicUnit/mbabuccaneer.txt", unitPlayer);
        map_production["TB-X Marauder"] = new WaterUnit(":/dynamic/dynamicUnit/tbxmarauder.txt", unitPlayer);
        map_production["W-1 Fortress"] = new WaterUnit(":/dynamic/dynamicUnit/w1fortress.txt", unitPlayer);

        for(auto &it : map_production)
        {
            connect(it.second->getUnitDisplay(), SIGNAL(unitDispl_clicked(Unit*)), this, SLOT(SLOT_setUnitToBuild(Unit*)));
            vector_unitStorage.push_back(it.second);
        }
    }
}

FactoryUnit::~FactoryUnit()
{
    map_production.clear();
}

QString FactoryUnit::getUnitToBuild() const
{
    return qstring_unitToBuild;
}
void FactoryUnit::setUnitToBuild(const QString unitTarget)
{
    qstring_unitToBuild = unitTarget;
	return;
}

bool FactoryUnit::action(HexagonMatchfield* hexTarget, const int )
{
    if(bool_unitUsed == true)
    {
        return false;
    }
    if(qstring_unitToBuild != "" && unitPlayer->getCurrentEnergieStorage() - map_production[qstring_unitToBuild]->getUnitCost() >= 0
            && hexTarget->getUnitStationed() == nullptr && map_production[qstring_unitToBuild]->moveTo(hexTarget) != -1)
    {
        produceUnit(hexTarget);
        resetBuildUnloadParameter();
        bool_unitUsed = true;
        return true;
    }
    resetBuildUnloadParameter();
    return false;
}

void FactoryUnit::produceUnit(HexagonMatchfield* hexTarget)
{
    hexTarget->setUnitStationed(map_production[qstring_unitToBuild]->createUnit());
    hexTarget->getUnitStationed()->setUnitUsed(true);
    unitPlayer->setCurrentEnergieStorage(unitPlayer->getCurrentEnergieStorage() - hexTarget->getUnitStationed()->getUnitCost());
    hexTarget->getUnitStationed()->setUnitCost(0);
}

Unit* FactoryUnit::createUnit()
{
    return new FactoryUnit(unitFile, true, unitPlayer);
}

void FactoryUnit::resetBuildUnloadParameter()
{
    if(qstring_unitToBuild != "")
    {
        map_production[qstring_unitToBuild]->getUnitDisplay()->setColor(Qt::black);
        map_production[qstring_unitToBuild]->getUnitDisplay()->setZValue(0);
        qstring_unitToBuild = "";
    }
}

//für die KI

void FactoryUnit::SLOT_setUnitToBuild(Unit *unit)
{
    resetBuildUnloadParameter();
    qstring_unitToBuild = unit->getUnitName();
}

