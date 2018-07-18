/*
 * Author: Manuel
 * Version: 0.3
 * Datum 07.12.2017
 */

#ifndef MENUEWIDGET_H
#define MENUEWIDGET_H

#include <QMainWindow>
#include <QStackedLayout>
#include "optionswidget.h"
#include "startmenuewidget.h"
#include "gamewidget.h"
#include "debugbrowser.h"

#include <QDesktopWidget>
#include <QPointer>

class OptionsWidget;

namespace Ui {
    class MenueWidget;
}

class MenueWidget : public QMainWindow
{
    Q_OBJECT
private:
    StartMenueWidget *ptr_mwStartMenueWidget;
    OptionsWidget *ptr_mwOptionsWidget;
    GameWidget *ptr_mwGameWidget;
    Game* ptr_runningGame;
    QPointer<DebugBrowser> ptr_debugBrowser;

public:
    Ui::MenueWidget *ui;

    explicit MenueWidget(QPointer<DebugBrowser> browser, QWidget *parent = 0 );
    ~MenueWidget();
    void resizeEvent(QResizeEvent*);

    //get und set Methoden
    StartMenueWidget* getPtr_mwStartMenueWidget() { return ptr_mwStartMenueWidget; }
    GameWidget* getPtr_mwGameWidget() { return ptr_mwGameWidget; }
    void setPtr_mwGameWidget(GameWidget* ptr_gameWidget) { ptr_mwGameWidget = ptr_gameWidget; }
    void setPtr_runningGame(Game *value);
    Game *getPtr_runningGame() const;
};

#endif // STARTMENUEWIDGET_H
