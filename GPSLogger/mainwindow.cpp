/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QtSerialPort/QSerialPort>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "consolewindow.h"
#include "settingsdialog.h"
#include "nmeaparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setWindowIcon(QIcon("images/gps.jpg"));
    //console = new Console;
    //console->setEnabled(false);
    //setCentralWidget(console);

    mpConsoleWindow = new ConsoleWindow;
    serial = new QSerialPort(this);
    settings = new SettingsDialog;
    mpNMEAParser = new NMEAParser();

    mWriteToTempData = false;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);

    ui->actionStart->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionStop->setEnabled(false);

    ui->actionConfigure->setEnabled(true);

    initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    //connect(mpConsoleWindow, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
}

MainWindow::~MainWindow()
{
    delete settings;
    delete serial;
    delete ui;
    delete mpNMEAParser;

    mpConsoleWindow->close();
    delete mpConsoleWindow;
}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {

        //console->setEnabled(true);
        //console->setLocalEchoEnabled(p.localEchoEnabled);

        mpConsoleWindow->show();
        mpConsoleWindow->setConsoleEnabled(true);
        mpConsoleWindow->setConsoleLocalEchoEnabled(p.localEchoEnabled);


        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);

        ui->actionStart->setEnabled(true);
        ui->actionPause->setEnabled(false);
        ui->actionStop->setEnabled(false);

        ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                   .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                   .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    //console->setEnabled(false);
    mpConsoleWindow->setConsoleEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);

    ui->actionStart->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionStop->setEnabled(false);

    ui->statusBar->showMessage(tr("Disconnected"));

    mTempData.clear();
}

void MainWindow::startRecording()
{
    mWriteToTempData = true;

    ui->actionStart->setEnabled(false);
    ui->actionPause->setEnabled(true);
    ui->actionStop->setEnabled(true);
}

void MainWindow::pauseRecording()
{
    mWriteToTempData = false;

    ui->actionStart->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->actionStop->setEnabled(true);
}

void MainWindow::stopRecording()
{
    mWriteToTempData = false;

    ui->actionStart->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->actionStop->setEnabled(false);

    QString fileNameOut = QFileDialog::getSaveFileName(this, tr("Save NMEA data file"), "", tr("NMEA Files (*.nmea)"));

    QFile fileOut(fileNameOut);

    if (!fileOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        if (fileNameOut != "") {
            QString message = "Can't open file, " + fileNameOut + "!\nTry to save in other location.";
            QMessageBox::critical(this, tr("Error"), message);
        } else {
            QString message = "Data save was <b>canceled</b>!<br><br>Recorded data are still on buffer!";
            QMessageBox::warning(this, tr("Warning"), message);
        }

        ui->actionStart->setEnabled(true);
        ui->actionPause->setEnabled(false);
        ui->actionStop->setEnabled(true);
        return;
    }

    fileOut.write(mTempData);
    mTempData.clear();
}

void MainWindow::about()
{
    QString str;

    str  = "The <b>GPS Logger</b> use the Qt Serial Port module in modern GUI applications using QT platform.<br><br>";
    str += "Program can be distributed under GNU GPL v3.0 license.<br><br>";
    str += "Version: <b>0.001</b><br><br>";
    str += "Copyrights &copy; 2015 <b>Aleksander GRM</b>";


    QMessageBox::about(this, tr("About GPS Logger"), str);
}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    mpConsoleWindow->putData(data);
    paintMonitors(data);

    if( mWriteToTempData ) {
        QTextStream ss(data);

        while( !ss.atEnd() ) {
            QString line = ss.readLine().trimmed() + "\n";
            mTempData.append(line);
        }
    }


}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionStart, SIGNAL(triggered()), this, SLOT(startRecording()));
    connect(ui->actionPause, SIGNAL(triggered()), this, SLOT(pauseRecording()));
    connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(stopRecording()));

    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionClear, SIGNAL(triggered()), mpConsoleWindow, SLOT(clear()));

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    connect(ui->actionSerialOutput, SIGNAL(triggered()), this, SLOT(showSerialOutputWindow()) );
}

void MainWindow::showSerialOutputWindow()
{
    mpConsoleWindow->clear();
    mpConsoleWindow->show();
}

void MainWindow::paintMonitors(QByteArray const& data)
{
    mpNMEAParser->setData(data);

    QPalette palette;
    if ( mpNMEAParser->isGpsFixValid() ) {
        palette.setColor(QPalette::Base,Qt::white);
        palette.setColor(QPalette::Text,Qt::black);
    } else {
        palette.setColor(QPalette::Base,Qt::red);
        palette.setColor(QPalette::Text,Qt::white);
    }

    ui->gpsFix->setPalette(palette);
    ui->gpsFix->setText(mpNMEAParser->getGpsMonitor().fix);
    ui->gpsTime->setText(mpNMEAParser->getGpsMonitor().time);
    ui->gpsLat->setText(mpNMEAParser->getGpsMonitor().latatitude);
    ui->gpsLong->setText(mpNMEAParser->getGpsMonitor().longitude);
    ui->gpsSatellites->setText(mpNMEAParser->getGpsMonitor().numOfSatellites);
    ui->gpsError->setText(mpNMEAParser->getGpsMonitor().precision);
    ui->gpsLat->setText(mpNMEAParser->getGpsMonitor().latatitude);

    ui->sonarDepth->setText(mpNMEAParser->getSonarMonitor().depth);
    ui->sonarTemp->setText(mpNMEAParser->getSonarMonitor().temperature);
    ui->sonarSpeed->setText(mpNMEAParser->getSonarMonitor().speed);
}
