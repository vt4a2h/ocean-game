#-------------------------------------------------
#
# Project created by QtCreator 2014-03-03T20:19:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameOceane
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    grid.cpp \
    gameocean.cpp

HEADERS  += mainwindow.h \
    grid.h \
    gameocean.h

FORMS    += mainwindow.ui

RESOURCES += \
    main.qrc

CONFIG += c++11
