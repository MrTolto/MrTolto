#-------------------------------------------------
#
# Project created by QtCreator 2013-03-19T11:24:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TicTacToeGame
TEMPLATE = app


SOURCES += main.cpp \
    newgamewidget.cpp \
    tictactoewidget.cpp \
    tictactoemodel.cpp \
    savegamewidget.cpp \
    loadgamewidget.cpp \
    tictactoedataaccess.cpp

HEADERS  += \
    newgamewidget.h \
    tictactoewidget.h \
    tictactoemodel.h \
    savegamewidget.h \
    loadgamewidget.h \
    tictactoedataaccess.h

FORMS    +=
