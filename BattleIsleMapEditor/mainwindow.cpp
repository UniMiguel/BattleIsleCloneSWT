//-------------------
//Autor: Miguel
//letzte Änderung von Miugel
//letzte Änderung: 21.01.2018
//version 1.1
//mainwindow.cpp
//------------------

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    hexfield(new QGraphicsScene(this)),
    menuefield(new QGraphicsScene(this)),
    hexType(""), hexCacheField(nullptr), hexCacheMenue(nullptr), hexCacheUnit(nullptr),
    sizeX(15), sizeY(10), scaleFak(1.0), boltCach(0), fillRect(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->hexfieldGraphic->setScene(hexfield);
    ui->menueView->setScene(menuefield); 
    ui->fillingLabel->setText("False");

    connect(ui->spinBoxX, SIGNAL(valueChanged(int)), this, SLOT(spinBoxX_valueChanged(int)));
    connect(ui->spinBoxY, SIGNAL(valueChanged(int)), this, SLOT(spinBoxY_valueChanged(int)));
    connect(ui->HexMenueBut, SIGNAL(clicked(bool)), this, SLOT(on_HexMenueBut_clicked()));
    connect(ui->UnitMenueBut, SIGNAL(clicked(bool)), this, SLOT(on_UnitMenueBut_clicked()));
    connect(ui->ResetButton, SIGNAL(clicked(bool)), this, SLOT(on_ResetButton_clicked()));
    connect(ui->spinBoltanium,SIGNAL(valueChanged(int)), this, SLOT(on_spinBoltanium_valueChanged(int)));
    connect(ui->deleteBut, SIGNAL(clicked(bool)), this, SLOT(on_deleteBut_clicked()));
    connect(ui->actionSpeichern, SIGNAL(triggered(bool)),this, SLOT(speichern_triggerd()));
    connect(ui->actionSpeichernForBIC, SIGNAL(triggered(bool)),this, SLOT(speichernForGame_triggerd()));
    connect(ui->actionLoad_Map, SIGNAL(triggered(bool)),this, SLOT(ladenMap_triggered()));
    connect(ui->actionNeue_Map, SIGNAL(triggered(bool)),this, SLOT(newMap_triggerd()));

    ui->spinBoxX->setValue(sizeX);
    ui->spinBoxY->setValue(sizeY);

    ui->lcdPlayer1->setPalette(Qt::blue);
    ui->lcdPlayer2->setPalette(Qt::red);
    ui->lcdBoltCach->setPalette(Qt::black);
    ui->lcdBoltTotal->setPalette(Qt::black);
    ui->lcdTotal->setPalette(Qt::black);

    ui->spinBoxX->setRange(1,150);
    ui->spinBoxY->setRange(1,150);
    ui->spinBoltanium->setRange(0,500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createHexfield(std::vector<std::vector<Hexagon*>> &hexagonGrid)
{


    for(unsigned int i = 0; i < hexagonGrid.size(); i++)
    {
        for(unsigned int j = 0; j < hexagonGrid[i].size(); j++)
        {
            Hexagon* hex = hexagonGrid[i][j];
            connect(hex,SIGNAL(SIGNAL_clicked(Hexagon*)),this,SLOT(slot_changeHexType(Hexagon*)));
            connect(hex,SIGNAL(SIGNAL_clicked(Hexagon*)),this,SLOT(slot_changeUnit(Hexagon*)));

            //Groesse der Bilder
            int PicSizeX = hex->pixmap().size().width();
            int PicSizeY = hex->pixmap().size().height();

            int PicCoordX, PicCoordY; //Koordinaten der Bilder
            if(i & 1)
            {
                PicCoordX = i * 2 * PicSizeX / 3;
                PicCoordY = j * PicSizeY + (PicSizeY / 2);
            }
            else
            {
                PicCoordX = i * 2 * PicSizeX / 3;
                PicCoordY = j * PicSizeY;
            }

            if(hex->getUnit_stationed() != nullptr)
            {
                hexfield->addItem(hex->getUnit_stationed());
                hex->getUnit_stationed()->setPos(scaleFak*PicCoordX, scaleFak*PicCoordY);
            }

           hex->setPos(scaleFak*PicCoordX, scaleFak*PicCoordY);
           hexfield->addItem(hex);
           hex->setScale(scaleFak);
        }
    }
    ui->hexfieldGraphic->setScene( hexfield );
}

void MainWindow::createMenue(std::vector<Hexagon*> &hexagonGrid)
{
    int i=0;
    for(auto & it : hexagonGrid)
    {
        connect(it,SIGNAL(SIGNAL_clicked(Hexagon*)),this,SLOT(slot_getHexType(Hexagon*)));
        it->setPos(0, i* it->pixmap().size().height());
        menuefield->addItem(it);
        i++;
    }
    ui->menueView->setScene(menuefield);
}

void MainWindow::createUnitfield(std::vector<std::vector<Hexagon *>> &hexagonGrid)
{

    for(unsigned int i = 0; i < hexagonGrid.size(); i++)
    {
        for(unsigned int j = 0; j < hexagonGrid[i].size(); j++)
        {
            Hexagon* hex = hexagonGrid[i][j];
            connect(hex,SIGNAL(SIGNAL_clicked(Hexagon*)),this,SLOT(slot_getUnit(Hexagon*)));

            //Groesse der Bilder
            int PicSizeX = hex->pixmap().size().width();
            int PicSizeY = hex->pixmap().size().height();

            int PicCoordX, PicCoordY; //Koordinaten der Bilder
            if(i & 1)
            {
                PicCoordX = i * 2 * PicSizeX / 3;
                PicCoordY = j * PicSizeY + (PicSizeY / 2);
            }
            else
            {
                PicCoordX = i * 2 * PicSizeX / 3;
                PicCoordY = j * PicSizeY;
            }

            if(hex->getUnit_stationed() != nullptr)
            {
                menuefield->addItem(hex->getUnit_stationed());
                hex->getUnit_stationed()->setPos(PicCoordX,PicCoordY);
            }

           hex->setPos(PicCoordX, PicCoordY);
           menuefield->addItem(hex);
        }

    }
    ui->menueView->setScene( menuefield );
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == 16777248)
    {
        fillRect = !fillRect;
    }
    else if(event->key() == Qt::Key_A)
    {
        on_rotateLBut_clicked();
    }
    else if(event->key() == Qt::Key_D)
    {
        on_rotateRBut_clicked();
    }
    setFillingLabel();
}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
        if(hexCacheField != nullptr)
        {
            hexCacheField->setHexColor(Qt::black);
            hexCacheField->setZValue(0);
            hexCacheField= nullptr;
        }
}

//getter und setter
int MainWindow::getSizeX() const
{
    return sizeX;
}

int MainWindow::getSizeY() const
{
    return sizeY;
}

std::vector<std::vector<Hexagon*>> MainWindow::getMyField() const
{
    return myField;
}

std::vector<std::vector<Hexagon*>> MainWindow::getMyUnit() const
{
    return myUnit;
}

 std::vector<Hexagon*> MainWindow::getMyMenue() const
 {
     return myMenue;
 }

 QString MainWindow::getHexType() const
 {
     return hexType;
 }

void MainWindow::setSizeX(int arg)
{
    sizeX = arg;
}

void MainWindow::setSizeY(int arg)
{
    sizeY = arg;
}

void MainWindow::setMyField(std::vector<std::vector<Hexagon*>> &hexagonGrid)
{
    myField = hexagonGrid;
}

void MainWindow::setMyUnit(std::vector<std::vector<Hexagon*>> &hexagonGrid)
{
    myUnit = hexagonGrid;
}

void MainWindow::setMyMenue(std::vector<Hexagon *> &hexagonGrid)
{
    myMenue = hexagonGrid;
}

void MainWindow::setHexType(QString type)
{
    hexType = type;
}

void MainWindow::setFillingLabel()
{
    if(fillRect == true)
    {
            ui->fillingLabel->setText("True");
    }else{
        ui->fillingLabel->setText("False");
    }
}
// Ende

void MainWindow::updateBolt()
{
    if(hexCacheField !=nullptr)
        ui->spinBoltanium->setValue(hexCacheField->getBoltaniumCurrent());
}

void MainWindow::updateLCD()
{
    int tmpP1=0;
    int tmpP2=0;
    int tmpTot=0;
    int tmpBolt=0;
    for(unsigned int i = 0; i < myField.size(); i++)
    {
        for(unsigned int j = 0; j < myField[i].size(); j++)
        {
            Hexagon* hex = myField[i][j];
            if(hex->getUnit_stationed() != nullptr)
            {
                if(hex->getUnit_stationed()->getPLY() == 1)
                {
                    tmpP1++;
                    tmpTot++;
                }
                else if(hex->getUnit_stationed()->getPLY() == 2)
                {
                tmpTot++;
                tmpP2++;
                }
                else
                    tmpTot++;
            }
            tmpBolt += hex->getBoltaniumCurrent();
        }
    }
    ui->lcdPlayer1->display(tmpP1);
    ui->lcdPlayer2->display(tmpP2);
    ui->lcdTotal->display(tmpTot);
    ui->lcdBoltTotal->display(tmpBolt);
}

void MainWindow::spinBoxX_valueChanged(int arg1)
{
    int tmp = myField.size();
    sizeX=arg1;
    if(tmp < sizeX)
    {
        for(int i = tmp; i < arg1; i++)
        {
            std::vector<Hexagon*> vectorHex;
            for( int j = 0; j < sizeY; j++ )
            {
                vectorHex.push_back(new Hexagon(QPoint(i,j), "waterDeep"));
            }
            myField.push_back(vectorHex);
        }
    }
    else if( tmp > sizeX)
    {
        for(int i = tmp; i > sizeX; i--)
        {
                myField.pop_back();
        }
    }
    for(auto & it : hexfield->items())
    {
        hexfield->removeItem(it);
    }
    createHexfield(myField);
}

void MainWindow::spinBoxY_valueChanged(int arg1)
{
   int tmp = myField[0].size();
   sizeY=arg1;
   if(tmp < sizeY)
   for(int i = 0 ; i < sizeX ; i++)
   {
       for(int j = tmp; j < sizeY; j++)
           myField[i].push_back(new Hexagon(QPoint(i,j), "waterDeep"));
   }
   else if(tmp > sizeY)
   {
       for(int i = 0 ; i < sizeX ; i++)
          {
              for(int j = tmp; j > sizeY; j--)
                  myField[i].pop_back();
          }
   }
   for(auto & it : hexfield->items())
   {
       hexfield->removeItem(it);
   }
   createHexfield(myField);
}

void MainWindow::slot_changeHexType(Hexagon* hex)
{
    if(hexType != "")
    {
        hex->setHexMatchfieldType(hexType);
    }
    if(hexCacheField != nullptr)
    {
        hexCacheField->setHexColor(Qt::black);
        hexCacheField->setZValue(0);

        if(hexType != "" && fillRect == true)
        {
            for(int x = std::min(hex->getQpoint_gridPosition().x(), hexCacheField->getQpoint_gridPosition().x()); x <= std::max(hex->getQpoint_gridPosition().x(), hexCacheField->getQpoint_gridPosition().x()); x++)
            {
                for(int y = std::min(hex->getQpoint_gridPosition().y(), hexCacheField->getQpoint_gridPosition().y()); y <= std::max(hex->getQpoint_gridPosition().y(), hexCacheField->getQpoint_gridPosition().y()); y++)
                {
                    myField[x][y]->setHexMatchfieldType(hexType);
                }
            }
            fillRect = false;
            setFillingLabel();
        }
    }

    hex->setHexColor(Qt::red);
    hex->setZValue(3);
    hexCacheField = hex;
    hexfield->update();
    updateBolt();
}

void MainWindow::slot_getHexType(Hexagon * hex)
{
    if(hexCacheMenue != nullptr)
    {
        hexCacheMenue->setHexColor(Qt::black);
        hexCacheMenue->setZValue(0);
    }
    hexType = hex->getHexMatchfieldType();
    hex->setHexColor(Qt::red);
    hex->setZValue(3);

    hexCacheMenue = hex;
    if(hexCacheField != nullptr)
        hexCacheField->setDeg(0);
    menuefield->update();
}

void MainWindow::on_HexMenueBut_clicked()
{
    menuefield->setSceneRect(0.0,0.0,0.0,0.0);

    if(hexCacheUnit != nullptr)
    {
        hexCacheUnit->setHexColor(Qt::black);
        hexCacheUnit->setZValue(0);
        hexCacheUnit= nullptr;
    }

    for(auto & it : menuefield->items())
    {
        menuefield->removeItem(it);
    }
    createMenue(myMenue);
}

void MainWindow::on_UnitMenueBut_clicked()
{
    menuefield->setSceneRect(1.0,1.0,150.0,1600.0);

    if(hexCacheMenue != nullptr)
    {
        hexCacheMenue->setHexColor(Qt::black);
        hexCacheMenue->setZValue(0);
        hexCacheMenue= nullptr;
        hexType="";
     }

    for(auto & it : menuefield->items())
    {
        menuefield->removeItem(it);
    }
    createUnitfield(myUnit);
}

void MainWindow::on_ResetButton_clicked()
{
    if(hexCacheMenue != nullptr)
    {
        hexCacheMenue->setHexColor(Qt::black);
        hexCacheMenue->setZValue(0);
        hexCacheMenue= nullptr;
        hexType="";
     }

    if(hexCacheUnit != nullptr)
    {
        hexCacheUnit->setHexColor(Qt::black);
        hexCacheUnit->setZValue(0);
        hexCacheUnit= nullptr;
    }

    if(hexCacheField != nullptr)
    {
        hexCacheField->setHexColor(Qt::black);
        hexCacheField->setZValue(0);
        hexCacheField= nullptr;
    }

    boltCach =0;
    on_getBoltButt_clicked();
    updateLCD();
    menuefield->update();
    hexfield->update();
}

void MainWindow::on_spinBoltanium_valueChanged(int arg1)
{
    if(hexCacheField != nullptr)
    {
        hexCacheField->setBoltaniumCurrent(arg1);
        hexfield->update();
        updateLCD();
    }
}

void MainWindow::slot_getUnit(Hexagon *hex)
{
    if(hexCacheUnit != nullptr)
    {
        hexCacheUnit->setHexColor(Qt::black);
        hexCacheUnit->setZValue(0);
    }
    hexCacheUnit = hex;
    hexCacheUnit->setHexColor(Qt::red);
    hexCacheUnit->setZValue(3);
    setHexType("");
}

void MainWindow::slot_changeUnit(Hexagon *hex)
{
    if(hexCacheUnit != nullptr && hexCacheUnit->getUnit_stationed() != nullptr)
    {
        if(hex->getUnit_stationed() != nullptr)
        {
            hexfield->removeItem(hex->getUnit_stationed());
        }

        QPoint tmpPoint = hex->getQpoint_gridPosition();
        QString tmpName = hexCacheUnit->getUnit_stationed()->getName();
        int tmpPly = hexCacheUnit->getUnit_stationed()->getPLY();
        Unit* tmp = new Unit(tmpPoint, tmpName, tmpPly);
        hex->setUnit_stationed(tmp);
        hex->getUnit_stationed()->setPos(hex->pos());
        hex->getUnit_stationed()->setScale(scaleFak);
        hexfield->addItem(hex->getUnit_stationed());
        fieldUnit.push_back(tmp);
    }
    updateLCD();
    hexfield->update();
}

void MainWindow::on_rotateRBut_clicked()
{
    if(hexCacheField != nullptr)
    {
        if(hexCacheField->getHexMatchfieldType() == "streetStraight" || hexCacheField->getHexMatchfieldType() == "streetStraight60d" || hexCacheField->getHexMatchfieldType() =="streetStraight120d")
        {
            hexCacheField->setDeg(hexCacheField->getDeg()+1);
            switch (hexCacheField->getDeg())
            {
            case 0: hexType = "streetStraight"; break;
            case 1: hexType = "streetStraight60d"; break;
            case 2: hexType = "streetStraight120d"; hexCacheField->setDeg(-1); break;
            }
        }
        else if(hexCacheField->getHexMatchfieldType() == "streetCurve" || hexCacheField->getHexMatchfieldType() == "streetCurve60d" || hexCacheField->getHexMatchfieldType() == "streetCurve120d" || hexCacheField->getHexMatchfieldType() == "streetCurve180d" || hexCacheField->getHexMatchfieldType() == "streetCurve240d" || hexCacheField->getHexMatchfieldType() == "streetCurve300d")
        {
            if(hexCacheField->getDeg() >= 3)
                hexCacheField->setDeg(-3);

            hexCacheField->setDeg(hexCacheField->getDeg()+1);
            switch (hexCacheField->getDeg())
            {
            case 0: hexType = "streetCurve"; break;
            case 1: hexType = "streetCurve60d"; break;
            case 2: hexType = "streetCurve120d"; break;
            case 3: hexType = "streetCurve180d"; hexCacheField->setDeg(-3); break;
            case -2: hexType = "streetCurve240d"; break;
            case -1: hexType = "streetCurve300d"; break;
            }
        }
        else if(hexCacheField->getHexMatchfieldType() == "streetCrossing" || hexCacheField->getHexMatchfieldType() == "streetCrossing60d")
        {
            if(hexCacheField->getDeg() >= 1)
                hexCacheField->setDeg(-1);

            hexCacheField->setDeg(hexCacheField->getDeg()+1);
            switch (hexCacheField->getDeg())
            {
            case 0: hexType = "streetCrossing"; break;
            case 1: hexType = "streetCrossing60d";hexCacheField->setDeg(-1); break;
            }
        }
        hexCacheField->setHexMatchfieldType(hexType);

        if(hexCacheMenue != nullptr)
            hexType = hexCacheMenue->getHexMatchfieldType();
        else
            hexType ="";

        hexfield->update();
    }
}

void MainWindow::on_rotateLBut_clicked()
{
    if(hexCacheField != nullptr)
    {
        if(hexCacheField->getHexMatchfieldType() == "streetStraight" || hexCacheField->getHexMatchfieldType() == "streetStraight60d" || hexCacheField->getHexMatchfieldType() =="streetStraight120d")
        {
            hexCacheField->setDeg(hexCacheField->getDeg()-1);
            switch (hexCacheField->getDeg())
            {
            case 0: hexType = "streetStraight"; break;
            case -1: hexType = "streetStraight120d"; break;
            case -2: hexType = "streetStraight60d"; hexCacheField->setDeg(1); break;
            }
        }
        else if(hexCacheField->getHexMatchfieldType() == "streetCurve" || hexCacheField->getHexMatchfieldType() == "streetCurve60d" || hexCacheField->getHexMatchfieldType() == "streetCurve120d" || hexCacheField->getHexMatchfieldType() == "streetCurve180d" || hexCacheField->getHexMatchfieldType() == "streetCurve240d" || hexCacheField->getHexMatchfieldType() == "streetCurve300d")
        {
            if(hexCacheField->getDeg() <= -3)
                hexCacheField->setDeg(3);

            hexCacheField->setDeg(hexCacheField->getDeg()-1);
            switch (hexCacheField->getDeg())
            {
            case 0: hexType = "streetCurve"; break;
            case 1: hexType = "streetCurve60d"; break;
            case 2: hexType = "streetCurve120d"; break;
            case -3: hexType = "streetCurve180d"; hexCacheField->setDeg(3); break;
            case -2: hexType = "streetCurve240d"; break;
            case -1: hexType = "streetCurve300d"; break;
            }
        }
        else if(hexCacheField->getHexMatchfieldType() == "streetCrossing" || hexCacheField->getHexMatchfieldType() == "streetCrossing60d")
        {
            if(hexCacheField->getDeg() <= -1)
                hexCacheField->setDeg(1);

            hexCacheField->setDeg(hexCacheField->getDeg()-1);
            switch (hexCacheField->getDeg())
            {
            case 0: hexType = "streetCrossing"; break;
            case -1: hexType = "streetCrossing60d";hexCacheField->setDeg(-1); break;
            }
        }
        hexCacheField->setHexMatchfieldType(hexType);

        if(hexCacheMenue != nullptr)
            hexType = hexCacheMenue->getHexMatchfieldType();
        else
            hexType ="";

        hexfield->update();
    }
}

void MainWindow::on_deleteBut_clicked()
{
    if(hexCacheField->getUnit_stationed() != nullptr)
    {
        hexfield->removeItem(hexCacheField->getUnit_stationed());
        hexCacheField->setUnit_stationed(nullptr);
        updateLCD();
    }
}

void MainWindow::on_zoomINBut_clicked()
{
    if(scaleFak <= 3)
    {
        if(scaleFak > 1)
            scaleFak += 0.5;
        else
            scaleFak += 0.1;
        for(auto & it : hexfield->items())
        {
            it->setScale(scaleFak);
            hexfield->removeItem(it);
        }
        createHexfield(myField);
    }
}

void MainWindow::on_zoomOutBut_clicked()
{
    if(scaleFak >= 0.5)
    {
        if(scaleFak >1)
            scaleFak -= 0.5;
        else
            scaleFak -= 0.1;
        for(auto & it : hexfield->items())
        {
            it->setScale(scaleFak);
            hexfield->removeItem(it);
        }
        createHexfield(myField);
    }
}
void MainWindow::on_deleteAllBut_clicked()
{
    for(unsigned int i = 0; i < myField.size(); i++)
    {
        for(unsigned int j = 0; j < myField[i].size(); j++)
        {
            if( myField[i][j]->getUnit_stationed() != nullptr)
            {
                hexfield->removeItem(myField[i][j]->getUnit_stationed());
                myField[i][j]->setUnit_stationed(nullptr);
            }
        }
    }
    updateLCD();
    hexfield->update();
}

void MainWindow::on_deleteBoltBut_clicked()
{
    hexCacheField->setBoltaniumCurrent(0);
    updateBolt();
    updateLCD();
}

void MainWindow::on_deleteAllBoltBut_clicked()
{
    for(unsigned int i = 0; i < myField.size(); i++)
    {
        for(unsigned int j = 0; j < myField[i].size(); j++)
        {
            myField[i][j]->setBoltaniumCurrent(0);
        }
    }
    updateLCD();
    hexfield->update();
}

void MainWindow::on_getBoltButt_clicked()
{
    if(hexCacheField != nullptr)
    {
        boltCach = hexCacheField->getBoltaniumCurrent();
        ui->lcdBoltCach->display(boltCach);
    }

}

void MainWindow::on_setBoltButt_clicked()
{
    hexCacheField->setBoltaniumCurrent(boltCach);
    updateBolt();
    updateLCD();
}


void MainWindow::speichern_triggerd()
{
     QString fileName = QFileDialog::getSaveFileName(this,
             tr("Save File"), "",
             tr("Data Text (*.txt);;All Files (*)"));
     if (fileName.isEmpty())
              return;
          else {
              QFile file(fileName);
              if (!file.open(QIODevice::WriteOnly)) {
                  QMessageBox::information(this, tr("Unable to open file"),
                  //Bei Fehler Fehlerdialog ausgeben
                      file.errorString());
                  return;
              }
       QTextStream out(&file);
       out << -1 << " " << sizeX << " " << sizeY << "\n";
       for(unsigned int i = 0; i < myField.size(); i++)
       {
           for(unsigned int j = 0; j < myField[i].size(); j++)
           {
               Hexagon* hex = myField[i][j];

               out << hex->getHexMatchfieldType() << " "<< hex->getBoltaniumCurrent() << " ";
               if(hex->getUnit_stationed() != nullptr)
               {
                   out << 1 << " " << hex->getUnit_stationed()->getPLY() << " " << hex->getUnit_stationed()->getName() << "\n";
               }
               else
                   out << 0 << "\n";
           }
       }
        out << sizeX%sizeY;
     }

}

void MainWindow::speichernForGame_triggerd()
{
     QString fileName = QFileDialog::getSaveFileName(this,
             tr("Save File"), "",
             tr("Data Text (*.txt);;All Files (*)"));
     if (fileName.isEmpty())
              return;
          else {
              QFile file(fileName);
              if (!file.open(QIODevice::WriteOnly)) {
                  QMessageBox::information(this, tr("Unable to open file"),
                  //Bei Fehler Fehlerdialog ausgeben
                      file.errorString());
                  return;
              }
       QTextStream out(&file);
       out << sizeX << " " << sizeY << "\n";
       for(unsigned int i = 0; i < myField.size(); i++)
       {
           for(unsigned int j = 0; j < myField[i].size(); j++)
           {
               Hexagon* hex = myField[i][j];

               out << hex->getHexMatchfieldType() << " "<< hex->getBoltaniumCurrent() << " ";
               if(hex->getUnit_stationed() != nullptr)
               {
                   out << 1 << " " << hex->getUnit_stationed()->getPLY() << " "
                       << hex->getUnit_stationed()->getUnitType() << " " << hex->getUnit_stationed()->getFilepath() << "\n";
               }
               else
                   out << 0 << "\n";
           }
       }

     }

}

void MainWindow::ladenMap_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open File"), "",
            tr("Data Text (*.txt);;All Files (*)"));
           //Voreinstellungen für den Laden_Dialog
    if (fileName.isEmpty())
             return;
    else
    {

             QFile file(fileName);

             if (!file.open(QIODevice::ReadOnly)) {
                 QMessageBox::information(this, tr("Unable to open file"),
                    //Bei Fehler Fehlerdialog ausgeben
                     file.errorString());
                 return;
             }

             QTextStream in(&file);
             int tmp_E;

             in >> tmp_E;
             if(tmp_E == -1)
             {
                  in>> sizeX >> sizeY;
                  std::vector<std::vector<Hexagon*>> hexagonGameGrid;
                  for( int i = 0; i < sizeX; i++ )
                  {
                      std::vector<Hexagon*> vectorHex;
                      for( int j = 0; j < sizeY; j++ )
                      {
                          //---------------
                          int tmp_UnitBool;
                          int tmp_Bolt;
                          int tmp_ply;
                          QString tmp_UnitTyp;
                          QString tmp_Typ;
                          Unit* tmp_Unit;
                          Hexagon* tmp_Hex;
                          // --------------
                          in >> tmp_Typ >>tmp_Bolt >> tmp_UnitBool;
                          if(tmp_UnitBool ==1 )
                          {
                              in >> tmp_ply >> tmp_UnitTyp;
                              tmp_Unit = new Unit(QPoint(i,j), tmp_UnitTyp, tmp_ply);
                          }
                          else if(tmp_UnitBool == 0)
                          {
                              tmp_Unit = nullptr;
                          }
                          else
                          {
                              QMessageBox::information(this, tr("Unable to open file"),
                                  file.errorString());
                              return;
                          }
                          tmp_Hex = new Hexagon(QPoint(i,j), tmp_Typ, tmp_Unit);
                          tmp_Hex->setBoltaniumCurrent(tmp_Bolt);
                          vectorHex.push_back(tmp_Hex);
                      }
                      hexagonGameGrid.push_back(vectorHex);
                   }
                  in >> tmp_E;
                  if(tmp_E != sizeX%sizeY)
                  {
                      QMessageBox::information(this, tr("Unable to open file"),
                          file.errorString());
                      return;
                  }
                  setMyField(hexagonGameGrid);
                  for(auto & it : hexfield->items())
                  {
                      hexfield->removeItem(it);
                  }
                  createHexfield(hexagonGameGrid);
             }
             else
             {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
             }
    }
}

void MainWindow::newMap_triggerd()
{
    for(auto & it : hexfield->items())
    {
        hexfield->removeItem(it);
    }
    sizeX= 15;
    sizeY= 10;
    std::vector<std::vector<Hexagon*>> hexagonGameGrid;
    for( int i = 0; i < sizeX; i++ )
    {
        std::vector<Hexagon*> vectorHex;
        for( int j = 0; j < sizeY; j++ )
        {
            vectorHex.push_back(new Hexagon(QPoint(i,j), "waterDeep"));
        }
        hexagonGameGrid.push_back(vectorHex);
    }
    setMyField(hexagonGameGrid);
    createHexfield(hexagonGameGrid);
    updateLCD();
}
