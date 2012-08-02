#include "apptoolsmenu.h"
#include "util/iconutil.h"
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
    toolsCompareDataAction=toolsMenu->addAction(IconUtil::getIcon("compare_data"), tr("Compare &data"), this, SLOT(addDataComparer()));
    toolsCompareDataAction->setStatusTip(tr("Generate DML statements to make tables have the same data"));

    toolsMenu->addSeparator();

    toolsOptionsAction=toolsMenu->addAction(IconUtil::getIcon("settings"), tr("&Options..."));
    toolsOptionsAction->setStatusTip(tr("Application settings"));
}
