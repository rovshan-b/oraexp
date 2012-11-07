#include "tablecreatorgrants.h"
#include "../tablecreatortabs.h"
#include "widgets/objectgrantseditortable.h"
#include <QtGui>

TableCreatorGrants::TableCreatorGrants(const QString &schemaName, TableCreatorTabs *tableCreator, bool editMode, QWidget *parent) :
    DbObjectCreatorTab<TableCreatorTabs>(tableCreator, editMode, parent),
    schemaName(schemaName)
{
    QVBoxLayout *layout=new QVBoxLayout();

    grantsEditor=new ObjectGrantsEditorTable(editMode,
                                             OraExp::ObjectGrants,
                                             DbTreeModel::Table,
                                             schemaName);

    table=grantsEditor->table();

    grantsEditor->toolBar()->addAdvancedOptionsButton(this, SLOT(showAdvancedOptions(bool)));

    layout->addWidget(grantsEditor);

    layout->setContentsMargins(2, 2, 2, 0);

    this->setLayout(layout);

    connect(grantsEditor, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
}

void TableCreatorGrants::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectCreatorTab<TableCreatorTabs>::setQueryScheduler(queryScheduler);

    grantsEditor->setQueryScheduler(queryScheduler);
    showAdvancedOptions(false);
}

void TableCreatorGrants::setTableInfo(TableInfo *tableInfo)
{
    grantsEditor->populateTable(&tableInfo->grants);
}

void TableCreatorGrants::removeIncorrectRows()
{
    grantsEditor->removeIncorrectRows();
}

void TableCreatorGrants::showAdvancedOptions(bool show)
{
    table->setUpdatesEnabled(false);
    //show or hide last 4 columns
    int columnCount=table->horizontalHeader()->count();
    for(int i=columnCount-4; i<columnCount; ++i){
        table->setColumnHidden(i, !show);
    }
    table->setUpdatesEnabled(true);

}

QList<ObjectGrantInfo> TableCreatorGrants::getGrantsInfo() const
{
    return grantsEditor->getList();
}

void TableCreatorGrants::currentSchemaChanged(const QString &/*oldSchemaName*/, const QString &newSchemaName)
{
    grantsEditor->setInitialUserOrObjectName(newSchemaName);
}
