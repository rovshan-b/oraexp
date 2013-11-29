#include <QtGui/QApplication>
#include "mainwindow.h"
#include "proxystyle.h"
#include "util/metatypesutil.h"

#include "connectivity/dbconnection.h"
#include "connectivity/ociexception.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new ProxyStyle());

    try{
        DbConnection::initEnvironment();
    }catch(OciException &ex){
        QMessageBox::critical(0, QObject::tr("Failed to initialize"),
                              QObject::tr("Failed to initalize OCI environment. Most probably Oracle client libraries are not installed"
                                 " or installed incorrectly. %1").arg(ex.getErrorMessage()));
        return 0;
    }

    MetaTypesUtil::registerMetaTypes();

    QCoreApplication::setOrganizationName("OraExp");
    QCoreApplication::setOrganizationDomain("oraexp.com");
    QCoreApplication::setApplicationName("OraExp");

    MainWindow w;
    w.show();

    return a.exec();
}
