//-------------------
//Projekt: Battle Isle Clone
//         Map Editor
//         SWT- Praktikum - Gruppe 2
//Autor: Miguel
//letzte Änderung von Miugel
//letzte Änderung: 21.01.2018
//version 1.2
//main.cpp
//------------------

#include "mainwindow.h"
#include "hexagon.h"
#include "unit.h"
#include <vector>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //erstellung des feldes
    std::vector<std::vector<Hexagon*>> hexagonGameGrid;
    for( int i = 0; i < w.getSizeX(); i++ )
    {
        std::vector<Hexagon*> vectorHex;
        for( int j = 0; j < w.getSizeY(); j++ )
        {
            vectorHex.push_back(new Hexagon(QPoint(i,j), "waterDeep"));
        }
        hexagonGameGrid.push_back(vectorHex);
     }
     w.setMyField(hexagonGameGrid);
     w.createHexfield(hexagonGameGrid);

     std::vector<Hexagon*> hexagonMenueGrid;
     //Hexagon(QPoint (*,*), string type)
     //Mögliche Typen:
     //"waterDeep"           (Tiefes Meer)
     //"waterSeashore"       (Küste)
     //"forrest"             (Wald)
     //"grassland"           (Wiese)
     //"streetStraight"      (Straße Gerade)
     //"streetCurve"         (Straße mit Kurve)
     //"streetCrossing"      (Straßenkruzung)
     //"mountainTop"         (Bergspitze)
     //"mountainSideStraight"(BergseiteGrade)
     //"mountainSideCurve"   (BergseiteSchräg)
     //"boltanium"           (Boltanium)
     hexagonMenueGrid.push_back(new Hexagon(QPoint(0,0), "waterDeep"));
     hexagonMenueGrid.push_back(new Hexagon(QPoint(0,1), "waterSeashore"));
     hexagonMenueGrid.push_back(new Hexagon(QPoint(0,2), "forrest"));
     hexagonMenueGrid.push_back(new Hexagon(QPoint(0,3), "grassland"));
     hexagonMenueGrid.push_back(new Hexagon(QPoint(0,4), "streetStraight"));
     hexagonMenueGrid.push_back(new Hexagon(QPoint(1,0), "streetCurve"));
     hexagonMenueGrid.push_back(new Hexagon(QPoint(1,1), "streetCrossing"));
     hexagonMenueGrid.push_back(new Hexagon(QPoint(1,2), "mountainTop"));

     w.setMyMenue(hexagonMenueGrid);
     w.createMenue(hexagonMenueGrid);

     std::vector<std::vector<Hexagon*>> unitGrid;
     std::vector<Hexagon*> unitHexP1;
     std::vector<Hexagon*> unitHexP2;
     std::vector<Hexagon*> unitHexP0;
     for(int i = 0; i <= 2; i++ )
     {
         if(i==0)
         {
             //staticunit
             unitHexP0.push_back(new Hexagon(QPoint(i,0), "grassland", new Unit(QPoint(0,0), "headquater")));
             unitHexP0.push_back(new Hexagon(QPoint(i,1), "grassland", new Unit(QPoint(0,1), "factory")));
             unitHexP0.push_back(new Hexagon(QPoint(i,2), "grassland", new Unit(QPoint(0,2), "depot")));
         }
         else if(i==1)
         {
             int j = 0;
             //staticunit
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "headquater", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "factory", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "depot", i)));
             //groundunit
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "ben", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "ad9sphinx", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "favbuster", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "lucas", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "r1demon", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "scpmerlin", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "kevarn", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "t3scorpion", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "t4gladiator", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "mannuel", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "tlavinvader", i)));
             //airunit
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "mountainTop", new Unit(QPoint(i,j++), "derbolten", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "mountainTop", new Unit(QPoint(i,j++), "g2giant", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "mountainTop", new Unit(QPoint(i,j++), "xa7raven", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "mountainTop", new Unit(QPoint(i,j++), "xf7mosquito", i)));
             //waterunit
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "cvamazon", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "mbabuccaneer", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "tbxmarauder", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "msmiguel", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "w1fortress", i)));
             unitHexP1.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "z1pegasus", i)));
         }
         else
         {
             int j = 0;
             //staticunit
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "headquater", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "factory", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "depot", i)));
             //groundunit
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "ben", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "ad9sphinx", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "favbuster", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "lucas", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "r1demon", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "scpmerlin", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "kevarn", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "t3scorpion", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "t4gladiator", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "mannuel", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "grassland", new Unit(QPoint(i,j++), "tlavinvader", i)));
             //airunit
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "mountainTop", new Unit(QPoint(i,j++), "derbolten", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "mountainTop", new Unit(QPoint(i,j++), "g2giant", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "mountainTop", new Unit(QPoint(i,j++), "xa7raven", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "mountainTop", new Unit(QPoint(i,j++), "xf7mosquito", i)));
             //waterunit
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "cvamazon", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "mbabuccaneer", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "tbxmarauder", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "msmiguel", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "w1fortress", i)));
             unitHexP2.push_back(new Hexagon(QPoint(i,j++), "waterDeep", new Unit(QPoint(i,j++), "z1pegasus", i)));
         }
     }
     unitGrid.push_back(unitHexP1);
     unitGrid.push_back(unitHexP2);
     unitGrid.push_back(unitHexP0);
     w.setMyUnit(unitGrid);

    return a.exec();
}
