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
    ../textreaderbase.cpp \
    ../parsingtable.cpp \
    ../../grammar_parser/parsingtableaction.cpp \
    ../../grammar_parser/parsingtablerow.cpp \
    ../plsql/plsqlparsingtable.cpp

HEADERS  += mainwindow.h \
    ../codeparser.h \
    ../codescanner.h \
    ../codeparserfactory.h \
    ../plsql/plsqlparser.h \
    ../textcursorreader.h \
    ../plsql/plsqltokens.h \
    ../plsql/plsqlscanner.h \
    ../textreaderbase.h \
    ../parsingtable.h \
    ../../grammar_parser/parsingtableaction.h \
    ../../grammar_parser/parsingtablerow.h \
    ../plsql/plsqlparsingtable.h

FORMS    += mainwindow.ui
