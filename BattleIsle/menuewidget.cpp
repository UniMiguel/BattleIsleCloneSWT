/*
 * Author: Manuel
 * Version: 0.3
 * Datum 07.12.2017
 */
/*
 * Author: Lucas
 * Version: 0.4
 * Datum: 12.01.18
 * */

#include "menuewidget.h"
#include "ui_menuewidget.h"
#include <QDebug>
void MenueWidget::setPtr_runningGame(Game *value)
{
    if(ptr_runningGame != nullptr)
    {
        delete ptr_runningGame;
    }
    ptr_runningGame = value;
}

Game *MenueWidget::getPtr_runningGame() const
{
    return ptr_runningGame;
}

MenueWidget::MenueWidget(QPointer<DebugBrowser> browser, QWidget *parent) :
    QMainWindow(parent),
    ptr_debugBrowser(browser), ui(new Ui::MenueWidget)
{
    ui->setupUi(this);
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
    ptr_mwStartMenueWidget = new StartMenueWidget(this, this);
    ptr_mwOptionsWidget = new OptionsWidget(this, this, ptr_mwStartMenueWidget->getPtr_smwOptions());
    ptr_mwGameWidget = new GameWidget(this);
    ptr_runningGame = nullptr;

    //Füge die Verschiedenen Fenster dem MenueWidget zu
    ui->stack->addWidget(ptr_mwStartMenueWidget);
    ui->stack->addWidget(ptr_mwOptionsWidget);
    ui->stack->addWidget(ptr_mwGameWidget);
    ui->stack->setCurrentIndex(0);

    connect(ptr_mwStartMenueWidget, SIGNAL(SIGNAL_smwChangeIndexFromStack(int)), ui->stack, SLOT(setCurrentIndex(int)));
    connect(ptr_mwOptionsWidget, SIGNAL(SIGNAL_optChangeIndexFromStack(int)), ui->stack, SLOT(setCurrentIndex(int)));
    connect(ptr_mwGameWidget, SIGNAL(SIGNAL_gameWidChangeIndexFromStack(int)), ui->stack, SLOT(setCurrentIndex(int)));

    if(ptr_debugBrowser != nullptr)
    {
        ptr_debugBrowser->move(QApplication::desktop()->screen()->rect().center().x() - this->rect().center().x() + ptr_debugBrowser->width(),
                               QApplication::desktop()->screen()->rect().center().y() - this->rect().center().y());
    }
}

MenueWidget::~MenueWidget()
{
    delete ui;
    delete ptr_runningGame;
    delete ptr_mwStartMenueWidget;
    delete ptr_mwOptionsWidget;
    delete ptr_mwGameWidget;
    delete ptr_debugBrowser;
    qDebug() << "Destruktor MenueWidget end";
}

void MenueWidget::resizeEvent(QResizeEvent *event)
{
    if(ui->stack->currentIndex() == 2 && ptr_runningGame != nullptr)
    {
        ptr_mwGameWidget->resizeEvent(event, this->height(), this->width());
    }
}
