#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include "console.h"
#include <QWidget>

namespace Ui {
class ConsoleWindow;
}

class ConsoleWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleWindow(QWidget *parent = 0);
    ~ConsoleWindow();

    void putData(const QByteArray &data);
    void setConsoleEnabled(bool f);
    void setConsoleLocalEchoEnabled(bool f);

public slots:
    void clear();

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;


private:
    Ui::ConsoleWindow *ui;
    Console *mpConsole;
};

#endif // CONSOLEWINDOW_H
