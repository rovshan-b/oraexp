#include "apptoolsmenu.h"
#include "util/iconutil.h"
#include "connectionspane.h"
#include "dialogs/options_dialog/optionsdialog.h"
#include "mainwindow.h"
#include <QtGui>

AppToolsMenu::AppToolsMenu(QMenu *toolsMenu, QToolBar *toolbar, QObject *parent) : AppMainMenu(parent)
{
    setupMenu(toolsMenu, toolbar);
}

void AppToolsMenu::setupMenu(QMenu *toolsMenu, QToolBar *toolbar)
{
    toolbar->addSeparator();

    toolsCompareSchemasAction=toolsMenu->addAction(IconUtil::getIcon("compare_schemas"), tr("Compare &schemas"), this, SLOT(addSchemaComparer()));
    toolsCompareSchemasAction->setStatusTip(tr("Generate DDL statements to make target schema the same with current"));

    toolsExportSchemaAction=toolsMenu->addAction(IconUtil::getIcon("export_schema"), tr("Export sche&ma"), this, SLOT(addSchemaExporter()));
    toolsExportSchemaAction->setStatusTip(tr("Export schema objects to file"));

    toolsMenu->addSeparator();

    toolsCompareDataAction=toolsMenu->addAction(IconUtil::getIcon("compare_data"), tr("Compare &data"), this, SLOT(addDataComparer()));
    toolsCompareDataAction->setStatusTip(tr("Generate DML statements to make tables have the same data"));

    toolsCopyDataAction=toolsMenu->addAction(IconUtil::getIcon("copy_data"), tr("Cop&y data"), this, SLOT(addDataCopier()));
    toolsCopyDataAction->setStatusTip(tr("Copy table data to other schema or database"));

    toolsExportDataAction=toolsMenu->addAction(IconUtil::getIcon("export"), tr("E&xport data"), this, SLOT(addDataExporter()));
    toolsExportDataAction->setStatusTip(tr("Export table data to various formats"));

    toolsImportDataAction=toolsMenu->addAction(IconUtil::getIcon("import_data"), tr("I&mport data"), this, SLOT(addDataImporter()));
    toolsImportDataAction->setStatusTip(tr("Load data into table from CSV file"));

    toolsMenu->addSeparator();

    toolsObjectLookupAction=toolsMenu->addAction(IconUtil::getIcon("lookup_object"), tr("&Find object"), this, SLOT(showObjectLookupDialog()), QKeySequence("Ctrl+K"));
    toolbar->addAction(toolsObjectLookupAction);

    toolsMenu->addSeparator();

    toolsSessionBrowserAction=toolsMenu->addAction(IconUtil::getIcon("session_browser"), tr("&Session browser"), this, SLOT(addSessionBrowser()));
    toolsSessionBrowserAction->setStatusTip(tr("View currently connected sessions"));

    toolsMenu->addSeparator();

    toolsOptionsAction=toolsMenu->addAction(IconUtil::getIcon("settings"), tr("&Options..."), this, SLOT(showOptionsDialog()));
    toolsOptionsAction->setStatusTip(tr("Application settings"));

#ifdef DEBUG
    toolsMenu->addSeparator();
    toolsOptionsAction=toolsMenu->addAction("800x600", this, SLOT(resize800x600()));
    toolsOptionsAction=toolsMenu->addAction("1024x768", this, SLOT(resize1024x768()));
#endif
}

void AppToolsMenu::showOptionsDialog()
{
    OptionsDialog dialog(MainWindow::defaultInstance());
    dialog.exec();
}

#ifdef DEBUG
void AppToolsMenu::resize800x600()
{
    this->getConnectionsPane()->window()->resize(800, 540);
}

void AppToolsMenu::resize1024x768()
{
    this->getConnectionsPane()->window()->resize(1024, 740);
}
#endif

void AppToolsMenu::updateActionStates(ConnectionPage *cnPage, ConnectionPageTab * /*cnPageTab*/)
{
    toolsCompareSchemasAction->setEnabled(cnPage!=0);
    toolsExportSchemaAction->setEnabled(cnPage!=0);

    toolsCompareDataAction->setEnabled(cnPage!=0);
    toolsCopyDataAction->setEnabled(cnPage!=0);
    toolsExportDataAction->setEnabled(cnPage!=0);
    toolsImportDataAction->setEnabled(cnPage!=0);

    toolsObjectLookupAction->setEnabled(cnPage!=0);

    toolsSessionBrowserAction->setEnabled(cnPage!=0);
}
