//-------------------
//Autor: Miguel
//letzte Änderung von Miugel
//letzte Änderung: 21.01.2018
//version 1.1
//mainwindow.h
//------------------

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>
#include <QGraphicsView>
#include <QSpinBox>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <algorithm>
#include <QKeyEvent>
#include "hexagon.h"
#include "unit.h"

namespace Ui {
class MainWindow;
}

class Hexagon;
class Unit;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    QGraphicsScene *hexfield;
    QGraphicsScene *menuefield;
    std::vector<std::vector<Hexagon*>> myField;
    std::vector<std::vector<Hexagon*>> myUnit;
    std::vector<Hexagon*> myMenue;
    std::vector<Unit*> fieldUnit;
    QString hexType;
    int sizeX;
    int sizeY;
    double scaleFak;
    Hexagon* hexCacheField;
    Hexagon* hexCacheMenue;
    Hexagon* hexCacheUnit;
    int boltCach;

    bool fillRect;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createHexfield(std::vector<std::vector<Hexagon*>> &hexagonGrid);
    void createMenue(std::vector<Hexagon*> &hexagonGrid);
    void createUnitfield(std::vector<std::vector<Hexagon*>> &hexagonGrid);
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent*);

    int getSizeX() const;
    int getSizeY() const;
    std::vector<std::vector<Hexagon*>> getMyField() const;
    std::vector<std::vector<Hexagon*>> getMyUnit() const;
    std::vector<Hexagon*> getMyMenue() const;
    QString getHexType() const;


    void setSizeX(int);
    void setSizeY(int);
    void setMyField(std::vector<std::vector<Hexagon*>> &hexagonGrid);
    void setMyUnit(std::vector<std::vector<Hexagon*>> &hexagonGrid);
    void setMyMenue(std::vector<Hexagon*> &hexagonGrid);
    void setHexType(QString);
    void setFillingLabel();

    void updateBolt();
    void updateLCD();

signals:

private slots:
    void spinBoxX_valueChanged(int arg1);
    void spinBoxY_valueChanged(int arg1);
    void slot_changeHexType(Hexagon*);
    void slot_getHexType(Hexagon*);
    void on_HexMenueBut_clicked();
    void on_UnitMenueBut_clicked();
    void on_ResetButton_clicked();
    void on_spinBoltanium_valueChanged(int arg1);
    void slot_getUnit(Hexagon*);
    void slot_changeUnit(Hexagon*);
    void on_rotateLBut_clicked();
    void on_rotateRBut_clicked();
    void on_deleteBut_clicked();
    void on_zoomINBut_clicked();
    void on_zoomOutBut_clicked();
    void on_deleteAllBut_clicked();
    void on_deleteBoltBut_clicked();
    void on_deleteAllBoltBut_clicked();
    void on_getBoltButt_clicked();
    void on_setBoltButt_clicked();

    void speichern_triggerd();
    void speichernForGame_triggerd();
    void ladenMap_triggered();
    void newMap_triggerd();
};


#endif // MAINWINDOW_H
