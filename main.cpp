#include <QtGui/QApplication>
#include "mainwindow.h"
#include "proxystyle.h"
#include "util/metatypesutil.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new ProxyStyle());

    MetaTypesUtil::registerMetaTypes();

    QCoreApplication::setOrganizationName("OraExp");
    QCoreApplication::setOrganizationDomain("oraexp.com");
    QCoreApplication::setApplicationName("Oracle Explorer");

    MainWindow w;
    w.show();

    return a.exec();
}
