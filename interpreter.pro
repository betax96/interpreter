#-------------------------------------------------
#
# Project created by QtCreator 2016-07-13T14:43:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = interpreter
TEMPLATE = app

RC_FILE = interpreter.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    searchreplacedialog.cpp \
    goto.cpp \
    lexer.cpp \
    parser.cpp

HEADERS  += mainwindow.h \
    searchreplacedialog.h \
    goto.h \
    lexer.h \
    parser.h



FORMS    += mainwindow.ui \
    searchreplacedialog.ui \
    goto.ui

OTHER_FILES += lexer.l \
    parser.y


LIBS += -L$$PWD/ -licdelib

INCLUDEPATH += $$PWD/icde_incl
DEPENDPATH += $$PWD/icde_incl

RESOURCES += \
    icons.qrc
