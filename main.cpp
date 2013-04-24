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

#include "beans/columninfo.h"
#include "beans/constraintinfo.h"
#include "beans/indexinfo.h"
#include "beans/triggerinfo.h"
#include "beans/objectgrantinfo.h"
#include "beans/tableexternalinfo.h"

#include "grammar_parser/ebnfparser.h"
#include "grammar_parser/firstfollowsetcomputer.h"
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

    qRegisterMetaType< QList<ColumnInfo> >("QList<ColumnInfo>");
    qRegisterMetaType< QList<ConstraintInfo> >("QList<ConstraintInfo>");
    qRegisterMetaType< QList<IndexInfo> >("QList<IndexInfo>");
    qRegisterMetaType< QList<TriggerInfo> >("QList<TriggerInfo>");
    qRegisterMetaType< QList<ObjectGrantInfo> >("QList<ObjectGrantInfo>");
    qRegisterMetaType<TablePartitioningInfo>("TablePartitioningInfo");
    qRegisterMetaType<TableExternalInfo>("TableExternalInfo");
}

int main(int argc, char *argv[])
{
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
