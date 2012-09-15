#include <QtGui/QApplication>
#include "mainwindow.h"
#include "connectivity/dbconnection.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "connectivity/ociexception.h"
#include "navtree/dbtreeitemresult.h"
#include "util/queryexectask.h"
#include "beans/datacompareinfo.h"
#include "proxystyle.h"
//#include "beans/columninfo.h"

#include "grammar_parser/ebnfparser.h"
#include "grammar_parser/dfa.h"

void registerMetaTypes()
{
    qRegisterMetaType<DbConnection>("DbConnection");
    qRegisterMetaTypeStreamOperators<DbConnection>("DbConnection");

    qRegisterMetaType<QueryResult>("QueryResult");
    qRegisterMetaType<FetchResult>("FetchResult");
    qRegisterMetaType<DbTreeItemResult>("DbTreeItemResult");
    qRegisterMetaType<OciException>("OciException");

    qRegisterMetaType<QueryExecTask>("QueryExecTask");
    qRegisterMetaType< QList<QStringList> >("QList<QStringList>");

    qRegisterMetaType<DataCompareInfo>("DataCompareInfo");

    //qRegisterMetaType<ColumnInfo>("ColumnInfo");
}

int main(int argc, char *argv[])
{
    if(argc==2 && strcmp(argv[1], "dfa")==0){
        EBNFParser parser;
        parser.parse();
        DFA(parser.getBNFRules());
        return 0;
    }

    QApplication a(argc, argv);
    a.setStyle(new ProxyStyle());

    registerMetaTypes();

    QCoreApplication::setOrganizationName("OraExp");
    QCoreApplication::setOrganizationDomain("oraexp.com");
    QCoreApplication::setApplicationName("Oracle Explorer");

    MainWindow w;
    w.show();
    w.showConnectDialog();

    return a.exec();
}
