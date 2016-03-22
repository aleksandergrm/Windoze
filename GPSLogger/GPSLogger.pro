QT += widgets serialport

TARGET = GPSLogger
TEMPLATE = app
#RC_ICONS = images/gps.ico

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    consolewindow.cpp \
    nmeaparser.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    consolewindow.h \
    nmeaparser.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    consolewindow.ui

RESOURCES += \
    GPSLogger.qrc
