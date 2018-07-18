/*
 * Author: Manuel
 * Version: 0.3
 * Datum 07.12.2017
 */


#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>
#include "menuewidget.h"
#include "startmenuewidget.h"
#include "options.h"

class MenueWidget;
class StartMenueWidget;

namespace Ui {
class OptionsWidget;
}

class OptionsWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::OptionsWidget *ui;
    MenueWidget* ptr_optWidMenueWidget;
    Options* ptr_optWidOptions;

public:
    explicit OptionsWidget(QWidget *parent = 0, MenueWidget *parentMenueWidget = 0, Options *ptr_Options = 0);
    ~OptionsWidget();

    void returnToStartMenue();

private slots:
    void on_qbutton_apply_clicked();
    void on_qbutton_cancel_clicked();

    void on_qbutton_loadCustomMap_clicked();

signals:
    void SIGNAL_optChangeIndexFromStack(int);
};

#endif // OPTIONSWIDGET_H
