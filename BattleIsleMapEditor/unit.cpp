//-------------------
//Autor: Miguel
//letzte Änderung von Miugel
//letzte Änderung: 20.01.2018
//version 1.0
//unit.cpp
//------------------
#include "unit.h"

QString Unit::getUnitType() const
{
    return unitType;
}

Unit::Unit(QPoint qpoint, QString name, int i)
    : unitName(name), gridPos(qpoint), unitPLY(i)
{
    //staticunit
    if(name == "headquater")
    {
        filepath= ":/static/staticUnit/headquater.txt";
        switch(unitPLY)
        {
        case 0: setPixmap(QPixmap(":/img/StaticOhneRand/HQ_grau.png")); break;
        case 1: setPixmap(QPixmap(":/img/StaticOhneRand/HQ_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/StaticOhneRand/HQ_rot.png")); break;
        }
        unitType = "HEADQUATERUNIT";
    }
    if(name == "factory")
    {
        filepath= ":/static/staticUnit/factory.txt";
        switch(unitPLY)
        {
        case 0: setPixmap(QPixmap(":/img/StaticOhneRand/Factory_grau.png")); break;
        case 1: setPixmap(QPixmap(":/img/StaticOhneRand/Factory_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/StaticOhneRand/Factory_rot.png")); break;
        }
        unitType = "FACTORYUNIT";
    }
    if(name == "depot")
    {
        filepath= ":/static/staticUnit/depot.txt";
        switch(unitPLY)
        {
        case 0: setPixmap(QPixmap(":/img/StaticOhneRand/Lager_grau.png")); break;
        case 1: setPixmap(QPixmap(":/img/StaticOhneRand/Lager_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/StaticOhneRand/Lager_rot.png")); break;
        }
        unitType = "DEPOTUNIT";
    }
    //groundunit
    if(name == "ben")
    {
        filepath= ":/dynamic/dynamicUnit/ben.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/GroundOhneRand/AD-5_Blitz_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/GroundOhneRand/AD-5_Blitz_rot.png")); break;
        }
        unitType = "LIGHTUNIT";
    }
    if(name == "ad9sphinx")
    {
        filepath= ":/dynamic/dynamicUnit/ad9sphinx.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/GroundOhneRand/AD-9_Sphinx_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/GroundOhneRand/AD-9_Sphinx_rot.png")); break;
        }
        unitType = "HEAVYUNIT";
    }
    if(name == "favbuster")
    {
        filepath= ":/dynamic/dynamicUnit/favbuster.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/GroundOhneRand/FAV_Buster_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/GroundOhneRand/FAV_Buster_rot.png")); break;
        }
        unitType = "LIGHTUNIT";
    }
    if(name == "lucas")
    {
        filepath= ":/dynamic/dynamicUnit/lucas.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/GroundOhneRand/HG-12_Angel_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/GroundOhneRand/HG-12_Angel_rot.png")); break;
        }
        unitType = "MEDIUMUNIT";
    }
    if(name == "r1demon")
    {
        filepath= ":/dynamic/dynamicUnit/r1demon.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/GroundOhneRand/R-1_Demon_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/GroundOhneRand/R-1_Demon_rot.png")); break;
        }
        unitType = "LIGHTUNIT";
    }
    if(name == "scpmerlin")
    {
        filepath= ":/dynamic/dynamicUnit/scpmerlin.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/GroundOhneRand/SC-P_Merlin_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/GroundOhneRand/SC-P_Merlin_rot.png")); break;
        }
        unitType = "BUILDERUNIT";
    }
    if(name == "kevarn")
    {
        filepath= ":/dynamic/dynamicUnit/kevarn.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/GroundOhneRand/SC-T_Provider_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/GroundOhneRand/SC-T_Provider_rot.png")); break;
        }
        unitType = "TRANSPORTERGROUNDUNIT";
    }
    if(name == "t3scorpion")
    {
        filepath= ":/dynamic/dynamicUnit/t3scorpion.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/GroundOhneRand/T-3_Scorpion_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/GroundOhneRand/T-3_Scorpion_rot.png")); break;
        }
        unitType = "MEDIUMUNIT";
    }
    if(name == "t4gladiator")
    {
        filepath= ":/dynamic/dynamicUnit/t4gladiator.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/GroundOhneRand/T-4_Gladiator_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/GroundOhneRand/T-4_Gladiator_rot.png")); break;
        }
        unitType = "MEDIUMUNIT";
    }
    if(name == "mannuel")
    {
        filepath= ":/dynamic/dynamicUnit/mannuel.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/GroundOhneRand/T-7_Crusader_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/GroundOhneRand/T-7_Crusader_rot.png")); break;
        }
        unitType = "HEAVYUNIT";
    }
    if(name == "tlavinvader")
    {
        filepath= ":/dynamic/dynamicUnit/tlavinvader.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/GroundOhneRand/TLAV_Invader_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/GroundOhneRand/TLAV_Invader_rot.png")); break;
        }
        unitType = "HEAVYUNIT";
    }
    //airunit
    if(name == "derbolten")
    {
        filepath= ":/dynamic/dynamicUnit/derbolten.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/AirOhneRand/CAS_Firebird_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/AirOhneRand/CAS_Firebird_rot.png")); break;
        }
        unitType = "AIRUNIT";

    }
    if(name == "g2giant")
    {
        filepath= ":/dynamic/dynamicUnit/g2giant.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/AirOhneRand/G-2_Giant_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/AirOhneRand/G-2_Giant_rot.png")); break;
        }
        unitType = "TRANSPORTERAIRUNIT";

    }
    if(name == "xa7raven")
    {
        filepath= ":/dynamic/dynamicUnit/xa7raven.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/AirOhneRand/XA-7_Raven_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/AirOhneRand/XA-7_Raven_rot.png")); break;
        }
        unitType = "AIRUNIT";

    }
    if(name == "xf7mosquito")
    {
        filepath= ":/dynamic/dynamicUnit/xf7mosquito.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/AirOhneRand/XF-7_Mosquito_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/AirOhneRand/XF-7_Mosquito_rot.png")); break;
        }
        return;
        unitType = "AIRUNIT";
    }
    //waterunit
    if(name == "cvamazon")
    {
        filepath= ":/dynamic/dynamicUnit/cvamazon.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/WaterOhneRand/CV_Amazon_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/WaterOhneRand/CV_Amazon_rot.png")); break;
        }
        unitType = "WATERUNIT";
    }
    if(name == "mbabuccaneer")
    {
        filepath= ":/dynamic/dynamicUnit/mbabuccaneer.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/WaterOhneRand/MB-A_Buccaneer_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/WaterOhneRand/MB-A_Buccaneer_rot.png")); break;
        }
        unitType = "WATERUNIT";
    }
    if(name == "tbxmarauder")
    {
        filepath= ":/dynamic/dynamicUnit/tbcmarauder.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/WaterOhneRand/TB-X_Marauder_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/WaterOhneRand/TB-X_Marauder_rot.png")); break;
        }
        unitType = "WATERUNIT";
    }
    if(name == "msmiguel")
    {
        filepath= ":/dynamic/dynamicUnit/msmiguel.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/WaterOhneRand/U-7C_Barracuda_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/WaterOhneRand/U-7C_Barracuda_rot.png")); break;
        }
        unitType = "WATERUNIT";
    }
    if(name == "w1fortress")
    {
        filepath= ":/dynamic/dynamicUnit/w1fortress.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/WaterOhneRand/W-1_Fortress_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/WaterOhneRand/W-1_Fortress_rot.png")); break;
        }
        unitType = "WATERUNIT";
    }
    if(name == "z1pegasus")
    {
        filepath= ":/dynamic/dynamicUnit/z1pegasus.txt";
        switch(unitPLY)
        {
        case 1: setPixmap(QPixmap(":/img/WaterOhneRand/Z-1_Pegasus_blau.png")); break;
        case 2: setPixmap(QPixmap(":/img/WaterOhneRand/Z-1_Pegasus_rot.png")); break;
        }
        unitType = "TRANSPORTERWATERUNIT";
    }

    setPixmap(pixmap().scaled(64,64));
    setZValue(5);
}

QString Unit::getName()
{
    return unitName;
}

void Unit::setName(QString name)
{
    unitName = name;
}

QPoint Unit::getGridPos()
{
    return gridPos;
}

void Unit::setGridPos(QPoint p)
{
    gridPos = p;
}

int Unit::getPLY()
{
    return unitPLY;
}

void Unit::setPLY(int ply)
{
    unitPLY = ply;
}

QString Unit::getFilepath()
{
    return filepath;
}

void Unit::setFielpath(QString s)
{
    filepath = s;
}
