#ifndef DEBUGBROWSER_H
#define DEBUGBROWSER_H

#include <QDialog>

namespace Ui {
class DebugBrowser;
}

class DebugBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit DebugBrowser(QWidget *parent = 0);
    ~DebugBrowser();
    void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private slots:
    void on_pushbutton_clear_clicked();

private:
    Ui::DebugBrowser *ui;
};

#endif // DEBUGBROWSER_H
