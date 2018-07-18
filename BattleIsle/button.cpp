/*
 * Author: Lucas
 * Version: 0.1
 * Datum 01.12.2017
 *
 * Author: Manuel
 * version: 0.2
 * Datum: 12.01.2018
 * Kommentar: Buttons angepasst
 *
 * Author: Lucas
 * version: 0.3
 * Datum: 14.01.2018
 * Kommentar: Buttons angepasst
 * */

#include "button.h"
#include <QPainter>
/*Base Class: Button*/
Button::Button()
    : HexagonBase( QPoint(0,0) ), qpix_playerOne(QPixmap()), qpix_playerTwo(QPixmap())
{
}

void Button::changePixmapPlayerOne()
{
    setPixmap(qpix_playerOne.scaled(64,64));
}

void Button::changePixmapPlayerTwo()
{
    setPixmap(qpix_playerTwo.scaled(64,64));

}

/*ButtonMove*/
ButtonMove::ButtonMove(int sizeX, int sizeY)
    :Button()
{    
    qpix_playerOne = QPixmap(":/buttonRess/Buttons/MoveMit_blau.png");
    qpix_playerTwo = QPixmap(":/buttonRess/Buttons/MoveMit_rot.png");
    setPixmap(qpix_playerOne.scaled(sizeX,sizeY));
}

void ButtonMove::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked();
}

void ButtonMove::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
    if(!isEnabled())
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::black);

        painter->setOpacity(.5);
        painter->setBrush(brush);

        painter->drawPath(shape());
    }
}


void ButtonMove::printType()
{
    qDebug() << "ButtonMove";
}

/*ButtonAction*/
ButtonAction::ButtonAction(int sizeX, int sizeY)
    : Button()
{
    qpix_playerOne = QPixmap(":/buttonRess/Buttons/AttackeMit_blau.png");
    qpix_playerTwo = QPixmap(":/buttonRess/Buttons/AttackeMit_rot.png");
    setPixmap(qpix_playerOne.scaled(sizeX,sizeY));
}

void ButtonAction::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked();
}

void ButtonAction::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
    if(!isEnabled())
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::black);

        painter->setOpacity(.5);
        painter->setBrush(brush);

        painter->drawPath(shape());
    }
}

void ButtonAction::printType()
{
    qDebug() << "ButtonAction";
}

/*ButtonMap*/
ButtonMap::ButtonMap(int sizeX, int sizeY)
    : Button()
{
    qpix_playerOne = QPixmap(":/buttonRess/Buttons/Weltkarte.png");
    qpix_playerTwo = QPixmap(":/buttonRess/Buttons/Weltkarte.png");
    setPixmap(qpix_playerOne.scaled(sizeX,sizeY));
}

void ButtonMap::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked();
}

void ButtonMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
    if(!isEnabled())
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::black);

        painter->setOpacity(.5);
        painter->setBrush(brush);

        painter->drawPath(shape());
    }
}

void ButtonMap::printType()
{
    qDebug() << "ButtonMap";
}

/*ButtonInformation*/
ButtonInformation::ButtonInformation(int , int )
{
    //Noch implemetieren
    //QPixmap(""));
    //setPixmap(pixmap().scaled(sizeX,sizeY));
}

void ButtonInformation::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked();
}

void ButtonInformation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
    if(!isEnabled())
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::black);

        painter->setOpacity(.5);
        painter->setBrush(brush);

        painter->drawPath(shape());
    }
}

void ButtonInformation::printType()
{
    qDebug() << "ButtonInformation";
}

/*ButtonChangePhase*/
ButtonChangePhase::ButtonChangePhase(int sizeX, int sizeY)
    : Button()
{
    qpix_playerOne = QPixmap(":/buttonRess/Buttons/PhasenwechselMit_blau.png");
    qpix_playerTwo = QPixmap(":/buttonRess/Buttons/PhasenwechselMit_rot.png");
    setPixmap(qpix_playerOne.scaled(sizeX,sizeY));
}

void ButtonChangePhase::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked();
}

void ButtonChangePhase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
    if(!isEnabled())
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::black);

        painter->setOpacity(.5);
        painter->setBrush(brush);

        painter->drawPath(shape());
    }
}

void ButtonChangePhase::printType()
{
    qDebug() << "ButtonChangePhase";
}

/*ButtonMenue*/
ButtonMenue::ButtonMenue(int sizeX, int sizeY)
    : Button()
{
    qpix_playerOne = QPixmap(":/buttonRess/Buttons/RepairMit_blau.png");
    qpix_playerTwo = QPixmap(":/buttonRess/Buttons/RepairMit_rot.png");
    setPixmap(qpix_playerOne.scaled(sizeX,sizeY));
}

void ButtonMenue::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked();
}

void ButtonMenue::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
    if(!isEnabled())
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::black);

        painter->setOpacity(.5);
        painter->setBrush(brush);

        painter->drawPath(shape());
    }
}

void ButtonMenue::printType()
{
    qDebug() << "ButtonMenue";
}

/*ButtonZoomIn*/
ButtonZoomIn::ButtonZoomIn(int sizeX, int sizeY)
    : Button()
{
    qpix_playerOne = QPixmap(":/buttonRess/Buttons/ZoomIN_blau.png");
    qpix_playerTwo = QPixmap(":/buttonRess/Buttons/ZoomIN_rot.png");
    setPixmap(qpix_playerOne.scaled(sizeX,sizeY));
}

void ButtonZoomIn::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked();
}

void ButtonZoomIn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
    if(!isEnabled())
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::black);

        painter->setOpacity(.5);
        painter->setBrush(brush);

        painter->drawPath(shape());
    }
}

void ButtonZoomIn::printType()
{
    qDebug() << "ButtonMenue";
}

/*ButtonZoomOut*/
ButtonZoomOut::ButtonZoomOut(int sizeX, int sizeY)
    : Button()
{
    qpix_playerOne = QPixmap(":/buttonRess/Buttons/ZoomOUT_blau.png");
    qpix_playerTwo = QPixmap(":/buttonRess/Buttons/ZoomOUT_rot.png");
    setPixmap(qpix_playerOne.scaled(sizeX,sizeY));
}

void ButtonZoomOut::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked();
}

void ButtonZoomOut::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
    if(!isEnabled())
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::black);

        painter->setOpacity(.5);
        painter->setBrush(brush);

        painter->drawPath(shape());
    }
}

void ButtonZoomOut::printType()
{
    qDebug() << "ButtonMenue";
}
