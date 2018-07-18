/*
 * Author: Manuel
 * Version: 0.3
 * Datum 07.12.2017
 */


#include "optionswidget.h"
#include "ui_optionswidget.h"
#include <QFileDialog>
OptionsWidget::OptionsWidget( QWidget *parent, MenueWidget *parentMenueWidget, Options *ptr_Options ) :
    QWidget( parent ),
    ui( new Ui::OptionsWidget ),
    ptr_optWidMenueWidget( parentMenueWidget ),
    ptr_optWidOptions( ptr_Options )
{
    ui->setupUi(this);

    //Die Eingestellten Optionen anzeigen
    if( ptr_optWidOptions->getBool_ki() == true )
        ui->radioButton_computer->setChecked(true);
    else
        ui->radioButton_human->setChecked(true);

    if( ptr_optWidOptions->getBool_fogOfWar() == true )
        ui->radioButton_fogOfWarOn->setChecked(true);
    else
        ui->radioButton_fogOfWarOff->setChecked(true);

    if ( ptr_optWidOptions->getInt_roundLimit() == 4 )
        ui->comboBox_rounds->setCurrentIndex(1);
    else if ( ptr_optWidOptions->getInt_roundLimit() == 8 )
        ui->comboBox_rounds->setCurrentIndex(2);
    else if ( ptr_optWidOptions->getInt_roundLimit() == 16 )
        ui->comboBox_rounds->setCurrentIndex(3);
    else
        ui->comboBox_rounds->setCurrentIndex(0);

    new QListWidgetItem(tr("Random"), ui->listWidget_maps);
    //new QListWidgetItem(tr(":/test/map.txt"), ui->listWidget_maps);
    new QListWidgetItem(tr(":/test/CustomMap.txt"), ui->listWidget_maps);
    new QListWidgetItem(tr(":/test/OriginalMap1.txt"), ui->listWidget_maps);
    //new QListWidgetItem(tr(":/test/HQTestMap.txt"), ui->listWidget_maps);
    //new QListWidgetItem(tr(":/test/TransporterTextMap.txt"), ui->listWidget_maps);
}

OptionsWidget::~OptionsWidget()
{
    delete ui;
}

void OptionsWidget::returnToStartMenue()
{
    emit SIGNAL_optChangeIndexFromStack(0);
}

void OptionsWidget::on_qbutton_apply_clicked()
{
    //Die Einstellungen übertragen (in Klasse Options)
    if( ui->radioButton_computer->isChecked() )
        ptr_optWidOptions->setBool_ki(true);
    else
        ptr_optWidOptions->setBool_ki(false);

    if( ui->radioButton_fogOfWarOn->isChecked() )
        ptr_optWidOptions->setBool_fogOfWar(true);
    else
        ptr_optWidOptions->setBool_fogOfWar(false);

    if ( ui->comboBox_rounds->currentIndex() == 1 )
        ptr_optWidOptions->setInt_roundLimit(4);
    else if ( ui->comboBox_rounds->currentIndex() == 2 )
        ptr_optWidOptions->setInt_roundLimit(8);
    else if ( ui->comboBox_rounds->currentIndex() == 3 )
        ptr_optWidOptions->setInt_roundLimit(16);
    else
        ptr_optWidOptions->setInt_roundLimit(0);

    if(ui->listWidget_maps->currentItem() != nullptr)
    {
        ptr_optWidOptions->setStr_map(ui->listWidget_maps->currentItem()->text());
    }else{
        ptr_optWidOptions->setStr_map("");
    }

    (ptr_optWidMenueWidget->getPtr_mwStartMenueWidget())->smwUpdateSettings();
    returnToStartMenue();
}

void OptionsWidget::on_qbutton_cancel_clicked()
{
    //Zu den eingestellten Optionen zurückstellen
    if( ptr_optWidOptions->getBool_ki() == true )
        ui->radioButton_computer->setChecked(true);
    else
        ui->radioButton_human->setChecked(true);

    if( ptr_optWidOptions->getBool_fogOfWar() == true )
        ui->radioButton_fogOfWarOn->setChecked(true);
    else
        ui->radioButton_fogOfWarOff->setChecked(true);

    if ( ptr_optWidOptions->getInt_roundLimit() == 4 )
        ui->comboBox_rounds->setCurrentIndex(1);
    else if ( ptr_optWidOptions->getInt_roundLimit() == 8 )
        ui->comboBox_rounds->setCurrentIndex(2);
    else if ( ptr_optWidOptions->getInt_roundLimit() == 16 )
        ui->comboBox_rounds->setCurrentIndex(3);
    else
        ui->comboBox_rounds->setCurrentIndex(0);

    returnToStartMenue();
}

void OptionsWidget::on_qbutton_loadCustomMap_clicked()
{
    QString pathToLoadFile = QFileDialog::getOpenFileName(this);
    if(pathToLoadFile != "")
    {
        new QListWidgetItem(pathToLoadFile, ui->listWidget_maps);
    }
}
