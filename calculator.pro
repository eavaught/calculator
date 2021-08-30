QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    QCalcWidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    QCalcWidget.h \
    mainwindow.h

FORMS += \
    QCalcWidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
