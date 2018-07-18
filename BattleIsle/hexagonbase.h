/*
 * Author: Lucas
 * Version: 0.1
 * Datum 01.12.2017
 * */

#ifndef HEXAGONBASE_H
#define HEXAGONBASE_H

#include "enumerations.h"
#include <QGraphicsPixmapItem>

class HexagonBase : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
protected:
    QPoint qpoint_gridPosition; //Position im Grid
public:
    HexagonBase(QPoint qpoint_position);

    /*Müssen in Subklassen ausgeprägt werden*/
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) = 0;

    //get und set Methoden
    QPoint getQpoint_gridPosition() const;
};

#endif // HEXAGONBASE_H
