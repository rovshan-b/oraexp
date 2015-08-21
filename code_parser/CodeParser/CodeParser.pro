#-------------------------------------------------
#
# Project created by QtCreator 2012-12-25T19:55:16
#
#-------------------------------------------------

QT       += core gui

TARGET = CodeParser
TEMPLATE = app

DEFINES += OCI_IMPORT_LINKAGE OCI_CHARSET_WIDE CODE_PARSER_BUILD

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
    ../plsql/plsqlparsingtable.cpp \
    ../../util/strutil.cpp \
    ../../beans/tokeninfo.cpp \
    ../plsql/plsqltreebuilder.cpp \
    ../../beans/parsetreenode.cpp \
    ../stringreader.cpp \
    ../../grammar_parser/bnfruleoption.cpp \
    ../../beans/codecollapseposition.cpp \
    ../../beans/parsetreenodescope.cpp


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
    ../plsql/plsqlparsingtable.h \
    ../ireducelistener.h \
    ../../util/strutil.h \
    ../../enums.h \
    ../../beans/tokeninfo.h \
    ../plsql/plsqltreebuilder.h \
    ../../beans/parsetreenode.h \
    ../plsql/plsqlrules.h \
    ../stringreader.h \
    ../../grammar_parser/bnfruleoption.h \
    ../../beans/codecollapseposition.h \
    ../../beans/parsetreenodescope.h

include(GeneratedFiles.pri)


FORMS    += mainwindow.ui

INCLUDEPATH += /home/rovshan/Projects/Qt/OraExp

OTHER_FILES +=

RESOURCES += \
    ../../misc.qrc
