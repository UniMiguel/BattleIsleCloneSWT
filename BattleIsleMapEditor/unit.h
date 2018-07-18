//-------------------
//Autor: Miguel
//letzte Änderung von Miugel
//letzte Änderung: 20.01.2018
//version 1.0
//unit.h
//------------------

#ifndef UNIT_H
#define UNIT_H

#include <QString>
#include <QPixmap>
#include <QWidget>
#include <QGraphicsObject>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <cmath>
#include <QDebug>
#include "hexagon.h"
#include "mainwindow.h"

class Unit : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
private:
    QString unitName;
    QPoint gridPos;
    int unitPLY;
    QString filepath;
    QString unitType;
public:
    Unit(QPoint, QString, int = 0);

    QString getName();
    void setName(QString);
    QPoint getGridPos();
    void setGridPos(QPoint);
    int getPLY();
    void setPLY(int);
    QString getFilepath();
    void setFielpath(QString);
    QString getUnitType() const;
};

#endif // UNIT_H
