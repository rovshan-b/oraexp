#-------------------------------------------------
#
# Project created by QtCreator 2012-12-10T13:26:29
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = GrammarParser
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../bnfrule.cpp \
    ../bnfruleitem.cpp \
    ../dfa.cpp \
    ../dfaitem.cpp \
    ../dfastate.cpp \
    ../dfatransition.cpp \
    ../ebnfparser.cpp \
    ../ebnfscanner.cpp \
    ../ebnftoken.cpp \
    ../firstfollowsetcomputer.cpp \
    ../parsingtablebuilder.cpp \
    ../parsingtablerow.cpp \
    ../parsingtableaction.cpp \
    ../filewriter.cpp \
    ../util.cpp \
    ../bnfruleoption.cpp

HEADERS += \
    ../bnfrule.h \
    ../bnfruleitem.h \
    ../dfa.h \
    ../dfaitem.h \
    ../dfastate.h \
    ../dfatransition.h \
    ../ebnfparser.h \
    ../ebnfscanner.h \
    ../ebnftoken.h \
    ../firstfollowsetcomputer.h \
    ../parsingtablebuilder.h \
    ../parsingtablerow.h \
    ../parsingtableaction.h \
    ../filewriter.h \
    ../util.h \
    ../bnfruleoption.h \
    ../../typedefs.h
