#include "menuewidget.h"
#include "debugbrowser.h"
#include <QApplication>
#include <QPointer>

QPointer<MenueWidget> Menue;
QPointer<DebugBrowser> Browser;
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
        if(Browser)
        {
            Browser->outputMessage(type, context, msg);
        }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Browser = new DebugBrowser
    Browser = nullptr;
    Menue = new MenueWidget(Browser);

    //qInstallMessageHandler(myMessageOutput);

    Menue->show();
    if(Browser != nullptr)
    {Browser->show();}

    return a.exec();
}
