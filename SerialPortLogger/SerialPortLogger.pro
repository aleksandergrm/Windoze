QT += widgets serialport

TARGET = SerialPortLogger
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    filetype.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    filetype.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    filetype.ui

RESOURCES += \
    SerialPortLogger.qrc
