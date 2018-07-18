/*
 * Author: Manuel
 * Version: 0.2
 * Datum 07.12.2017
 */

#ifndef OPTIONS_H
#define OPTIONS_H
#include <QObject>
#include <string>
#include <QTextStream>

class Options : public QObject
{
    Q_OBJECT
private:
    bool bool_ki; //true = mit KI, false = ohne KI
    bool bool_fogOfWar; //true = mit Fog of War, false = ohne Fog of War
    QString str_map;
    int int_roundLimit; // 0 = kein Limit

public:
    Options();
    Options(bool bool_KI, bool bool_fog, QString str_m, int int_limit);
    void serialize(QTextStream&);
    static Options* unserialize(QTextStream&);

    //get und set Methoden
    bool getBool_ki() { return bool_ki; }
    bool getBool_fogOfWar() { return bool_fogOfWar; }
    QString getStr_map() { return str_map; }
    int getInt_roundLimit() { return int_roundLimit; }
    void setBool_ki( bool b ) { bool_ki = b; }
    void setBool_fogOfWar( bool b ) { bool_fogOfWar = b; }
    void setStr_map( QString s ) { str_map = s; }
    void setInt_roundLimit( int i ) { int_roundLimit = i; }

public slots:
};

#endif // OPTIONS_H
