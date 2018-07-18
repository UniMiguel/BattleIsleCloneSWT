/*
 * Author: Lucas
 * Version: 0.1
 * Datum 01.12.2017
 * */

#include "hexagonbase.h"

QPoint HexagonBase::getQpoint_gridPosition() const
{
    return qpoint_gridPosition;
}

HexagonBase::HexagonBase(QPoint qpoint_position)
    :  qpoint_gridPosition(qpoint_position)
{

}
