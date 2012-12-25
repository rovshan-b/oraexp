#-------------------------------------------------
#
# Project created by QtCreator 2012-12-25T19:55:16
#
#-------------------------------------------------

QT       += core gui

TARGET = CodeParser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../codeparser.cpp \
    ../codescanner.cpp \
    ../codeparserfactory.cpp \
    ../plsql/plsqlparser.cpp \
    ../textcursorreader.cpp

HEADERS  += mainwindow.h \
    ../codeparser.h \
    ../codescanner.h \
    ../codeparserfactory.h \
    ../plsql/plsqlparser.h \
    ../itextreader.h \
    ../textcursorreader.h \
    ../plsql/plsqldefines.h

FORMS    += mainwindow.ui
