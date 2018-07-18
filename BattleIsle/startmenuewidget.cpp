/*
 * Author: Manuel
 * Version: 0.3
 * Datum: 07.12.2017
 *
 * Author: Manuel
 * Version: 0.4
 * Datum: 30.12.2017
 * Kommentar: Verbindung zu GameWidget hinzugefuegt
 */

#include "startmenuewidget.h"
#include "ui_startmenuewidget.h"
//#include <QDebug>
#include <QFileDialog>

StartMenueWidget::StartMenueWidget(QWidget *parent , MenueWidget *ptr_menueWidget) :
    QWidget( parent ),
    ui( new Ui::StartMenueWidget ),
    ptr_smwMenueWidget(ptr_menueWidget)
{
    this->setWindowTitle("Battle Isle Clone");
    ui->setupUi( this );
    ptr_smwOptions = new Options();

    //Die eingestellten Optionen anzeigen
    if( ptr_smwOptions->getBool_ki() == true )
        ui->label_opponent_indication->setText( "Computer" );
    if( ptr_smwOptions->getBool_fogOfWar() == true )
        ui->label_fogOfWar_indication->setText( "Ein" );
    ui->label_map_indication->setText( ptr_smwOptions->getStr_map() );
    if( ptr_smwOptions->getInt_roundLimit() == 0)
        ui->label_limit_indication->setText("Kein Limit");
    else
        ui->label_limit_indication->setText(QString::number(ptr_smwOptions->getInt_roundLimit()));
}

StartMenueWidget::~StartMenueWidget()
{
    delete ui;
}

void StartMenueWidget::startGame()
{
    if(ptr_smwOptions != nullptr)
    {
        //Erstelle ein Objekt vom Typ Game
        ptr_smwMenueWidget->setPtr_runningGame(new Game(ptr_smwOptions, ptr_smwMenueWidget->getPtr_mwGameWidget()));
        connect(ptr_smwMenueWidget->getPtr_runningGame(), SIGNAL(gameOver()), this, SLOT(gameEnded()));

        ptr_smwMenueWidget->resize( 1200,780 );
        ptr_smwMenueWidget->ui->stack->resize( 1190, 740 );
        ptr_smwMenueWidget->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

        //Wechsel zu Widget 3
        emit SIGNAL_smwChangeIndexFromStack(2);
        //Das Signal wird im Konstruktor von MenueWidget verbunden
    }


}

void StartMenueWidget::loadGame(QString pathToLoadFile)
{
    //Erstelle ein Objekt vom Typ Game
    ptr_smwMenueWidget->setPtr_runningGame(new Game(pathToLoadFile, ptr_smwMenueWidget->getPtr_mwGameWidget()));
    connect(ptr_smwMenueWidget->getPtr_runningGame(), SIGNAL(gameOver()), this, SLOT(gameEnded()));

    ptr_smwMenueWidget->resize( 1200,750 );
    ptr_smwMenueWidget->ui->stack->resize( 1190, 740 );
    ptr_smwMenueWidget->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    //Wechsel zu Widget 3
    emit SIGNAL_smwChangeIndexFromStack(2);
    //Das Signal wird im Konstruktor von MenueWidget verbunden
}

void StartMenueWidget::switchToOptions()
{
    //Wechsel zu Widget 2
    emit SIGNAL_smwChangeIndexFromStack(1);
    //Das Signal wird im Konstruktor von MenueWidget verbunden
}
void StartMenueWidget::smwUpdateSettings()
{
    //Die eingestellten Optionen updaten
    if( ptr_smwOptions->getBool_ki() == true )
        ui->label_opponent_indication->setText( "Computer" );
    else
        ui->label_opponent_indication->setText( "Mensch" );

    if( ptr_smwOptions->getBool_fogOfWar() == true )
        ui->label_fogOfWar_indication->setText( "Ein" );
    else
        ui->label_fogOfWar_indication->setText( "Aus" );

    ui->label_map_indication->setText( ptr_smwOptions->getStr_map() );

    if( ptr_smwOptions->getInt_roundLimit() == 0 )
        ui->label_limit_indication->setText("Kein Limit");
    else
        ui->label_limit_indication->setText(QString::number(ptr_smwOptions->getInt_roundLimit()));
}

void StartMenueWidget::on_qbutton_startGame_clicked()
{
    startGame();
}

void StartMenueWidget::on_qbutton_loadGame_clicked()
{
    //switchToLoadGame();
    QString pathToLoadFile = QFileDialog::getOpenFileName(this);
    if(pathToLoadFile != "")
    {
        loadGame(pathToLoadFile);
    }
}

void StartMenueWidget::on_qbutton_options_clicked()
{
    switchToOptions();
}

void StartMenueWidget::gameEnded()
{
    ptr_smwMenueWidget->setPtr_runningGame(nullptr);
/*
    qDebug() << "Elemente in getGameWidButtonScene" <<ptr_smwMenueWidget->getPtr_mwGameWidget()->getGameWidButtonScene()->items().size();
    qDebug() << "Elemente in getGameWidGameScene" <<ptr_smwMenueWidget->getPtr_mwGameWidget()->getGameWidGameScene()->items().size();
    qDebug() << "Elemente in getGameWidInfoScene" <<ptr_smwMenueWidget->getPtr_mwGameWidget()->getGameWidInfoScene()->items().size();
    qDebug() << "Elemente in getGameWidMapScene" <<ptr_smwMenueWidget->getPtr_mwGameWidget()->getGameWidMapScene()->items().size();
    qDebug() << "Elemente in getGameWidOptionsScene" <<ptr_smwMenueWidget->getPtr_mwGameWidget()->getGameWidOptionsScene()->items().size();
    qDebug() << "Elemente in getGameWidMenueScene" <<ptr_smwMenueWidget->getPtr_mwGameWidget()->getGameWidMenueScene()->items().size();
*/
    ptr_smwMenueWidget->resize(400, 500);
    ptr_smwMenueWidget->ui->stack->resize(380, 440);
    emit SIGNAL_smwChangeIndexFromStack(0);
//    qDebug() << "\t Change Index Emit";
    ptr_smwMenueWidget->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
    //ptr_smwMenueWidget->repaint();
//    qDebug() << "\t resize";

}
