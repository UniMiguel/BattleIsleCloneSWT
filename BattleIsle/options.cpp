/*
 * Author: Manuel
 * Version: 0.2
 * Datum 07.12.2017
 * */
#include "options.h"
#include <QDebug>

/*StandardKonstruktor, kann noch angepasst werden*/
Options::Options()
    : Options(0, 1, "Random", 16)
{

}

Options::Options(bool bool_KI, bool bool_fog, QString str_m, int int_limit)
    : bool_ki(bool_KI), bool_fogOfWar(bool_fog), str_map(str_m), int_roundLimit(int_limit)
{

}

void Options::serialize(QTextStream &out)
{
    out << str_map << " " << bool_fogOfWar << " "
        << int_roundLimit << " " << bool_ki << "\n";
}

Options *Options::unserialize(QTextStream &in)
{
    QString map;
    QString str_fogOfWar;
    QString str_roundLimit;
    QString str_ki;

    bool fogOfWar;
    bool ki;
    int roundLimit;

    in >> map;
    in >> str_fogOfWar;
    in >> str_roundLimit;
    in >> str_ki;

    fogOfWar = (bool)str_fogOfWar.toInt();
    ki = (bool)str_ki.toInt();
    roundLimit = str_roundLimit.toInt();

    //qDebug() << "unserialize von options ausführen mit \n\t" << map << "\n\t" << fogOfWar << "\n\t" << roundLimit << "\n\t" << ki;

    return new Options(ki,fogOfWar,map,roundLimit);
}
