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
    ../textcursorreader.cpp \
    ../plsql/plsqlscanner.cpp \
    ../textreaderbase.cpp

HEADERS  += mainwindow.h \
    ../codeparser.h \
    ../codescanner.h \
    ../codeparserfactory.h \
    ../plsql/plsqlparser.h \
    ../textcursorreader.h \
    ../plsql/plsqltokens.h \
    ../plsql/plsqlscanner.h \
    ../textreaderbase.h

FORMS    += mainwindow.ui
