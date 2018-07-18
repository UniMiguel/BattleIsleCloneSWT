#-------------------------------------------------
#
# Project created by QtCreator 2017-12-01T21:38:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BattleIsle
TEMPLATE = app


SOURCES += main.cpp\
    hexagonbase.cpp \
    button.cpp \
    hexagonmatchfield.cpp \
    options.cpp \
    optionswidget.cpp \
    menuewidget.cpp \
    startmenuewidget.cpp \
    gamewidget.cpp \
    game.cpp \
    round.cpp \
    player.cpp \
    unit.cpp \
    staticunit.cpp \
    dynamicunit.cpp \
    ki.cpp \
    debugbrowser.cpp

HEADERS  += \
    hexagonbase.h \
    button.h \
    enumerations.h \
    hexagonmatchfield.h \
    options.h \
    optionswidget.h \
    menuewidget.h \
    startmenuewidget.h \
    gamewidget.h \
    game.h \
    unit.h \
    round.h \
    player.h \
    staticunit.h \
    dynamicunit.h \
    ki.h \
    debugbrowser.h

FORMS    += startmenuewidget.ui \
    matchfield.ui \
    optionswidget.ui \
    menuewidget.ui \
    startmenuewidget.ui \
    gamewidget.ui \
    debugbrowser.ui

RESOURCES += \
    img/rsc.qrc \
    UnitRessources/units.qrc \
    maps/predefinedmaps.qrc

CONFIG += c++11
