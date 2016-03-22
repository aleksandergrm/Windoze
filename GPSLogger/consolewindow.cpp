#include "consolewindow.h"
#include "ui_consolewindow.h"

ConsoleWindow::ConsoleWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleWindow)
{
    ui->setupUi(this);

    mpConsole = new Console(this);
    mpConsole->setEnabled(false);
}

ConsoleWindow::~ConsoleWindow()
{
    delete ui;
    mpConsole->close();
    delete mpConsole;
}

void
ConsoleWindow::putData(const QByteArray &data)
{
    mpConsole->putData(data);
}

void
ConsoleWindow::clear()
{
    mpConsole->clear();
}

void ConsoleWindow::setConsoleEnabled(bool f)
{
    mpConsole->setEnabled(f);
}

void ConsoleWindow::setConsoleLocalEchoEnabled(bool f)
{
    mpConsole->setLocalEchoEnabled(f);
}

void ConsoleWindow::resizeEvent(QResizeEvent *e)
{
    //mpConsole->resizeEvent(e);

    QRect cr = contentsRect();
    mpConsole->setGeometry(QRect(cr.left(), cr.top(), cr.width(), cr.height()));
}

