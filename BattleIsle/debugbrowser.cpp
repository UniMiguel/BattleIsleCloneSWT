#include "debugbrowser.h"
#include "ui_debugbrowser.h"
#include <QDesktopWidget>

DebugBrowser::DebugBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugBrowser)
{
    ui->setupUi(this);
}

DebugBrowser::~DebugBrowser()
{
    delete ui;
}

void DebugBrowser::outputMessage(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    switch (type)
    {
    case QtDebugMsg:
        ui->qtext_debugBrowser->append(tr("-> %1").arg(msg));
        break;
    case QtWarningMsg:
        ui->qtext_debugBrowser->append(tr("— WARNING: %1").arg(msg));
        break;
    case QtCriticalMsg:
        ui->qtext_debugBrowser->append(tr("— CRITICAL: %1").arg(msg));
        break;
    case QtInfoMsg:
        ui->qtext_debugBrowser->append(tr("— INFO: %1").arg(msg));
        break;
    case QtFatalMsg:
        ui->qtext_debugBrowser->append(tr("— FATAL: %1").arg(msg));
        abort();
    }
}

void DebugBrowser::on_pushbutton_clear_clicked()
{
    ui->qtext_debugBrowser->clear();
}
