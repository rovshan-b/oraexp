#include "dbobjectcomparercomparetab.h"
#include "util/widgethelper.h"
#include "widgets/connectionselectorwidget.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/subtabwidget.h"
#include "widgets/dbtreeviewpanel.h"
#include "connectivity/dbconnection.h"
#include <QtGui>

DbObjectComparerCompareTab::DbObjectComparerCompareTab(DbUiManager *uiManager, QWidget *parent) :
    QWidget(parent), uiManager(uiManager), parentQueryEndMonitor(parent),
    queryScheduler(0), sourceSchemaComboBox(0), bottomPaneTab(0)
{
}

void DbObjectComparerCompareTab::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(0, 2, 0, 2);

    createConnectionOptionsPane(mainLayout);

    if(nestOptionsTab()){
        bottomPaneTab = new SubTabWidget();
        QWidget *itemsTableWidget = new QWidget();

        QVBoxLayout *itemsTableLayout = new QVBoxLayout();
        //itemsTableLayout->setContentsMargins(0,0,0,0);
        itemsTableWidget->setLayout(itemsTableLayout);

        createItemsTable(itemsTableLayout);

        bottomPaneTab->addTab(itemsTableWidget, tr("Objects"));

        mainLayout->addWidget(bottomPaneTab);
    }else{
        createItemsTable(mainLayout);
    }

    setLayout(mainLayout);

    if(this->queryScheduler){
        loadDbObjects();
    }

    emit uiCreated();
}

void DbObjectComparerCompareTab::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    loadDbObjects();
}

void DbObjectComparerCompareTab::loadDbObjects()
{
    if(sourceSchemaComboBox==0){
        return;
    }

    WidgetHelper::setComboBoxText(sourceSchemaComboBox, initialSchemaName.isEmpty() ? queryScheduler->getDb()->getUsername() : initialSchemaName);
    sourceSchemaComboBox->loadItems(queryScheduler, "get_schema_list");

    connect(sourceSchemaComboBox->lineEdit(), SIGNAL(editingFinished()), this, SLOT(loadSchemaObjects()));
    connect(sourceSchemaComboBox->lineEdit(), SIGNAL(editingFinished()), this, SLOT(sourceSchemaNameChanged()));

    emit sourceSchemaChanged(queryScheduler->getDb()->getUsername());
    loadSchemaObjects();
}

void DbObjectComparerCompareTab::treeChildrenPopulated(const QModelIndex &parent)
{
    if(!initialObjectName.isEmpty()){
        DbTreeView *schemaObjectsTree = schemaObjectsPanel->tree();
        DbTreeModel *model = schemaObjectsTree->getModel();
        QModelIndex objectIndex = model->findByName(parent, initialObjectName);
        if(objectIndex.isValid()){
            model->checkItem(objectIndex, Qt::Checked, true);
            schemaObjectsTree->selectionModel()->select(objectIndex, QItemSelectionModel::SelectCurrent);
            schemaObjectsTree->scrollTo(objectIndex, QAbstractItemView::PositionAtCenter);
            currentTreeItemChanged(objectIndex, QModelIndex());
        }
    }
}

void DbObjectComparerCompareTab::currentTreeItemChanged(const QModelIndex &, const QModelIndex &)
{
}

void DbObjectComparerCompareTab::createConnectionOptionsPane(QBoxLayout *layout)
{
    topPaneForm=new QFormLayout();

    sourceSchemaComboBox=new DbItemListComboBox("", "user", true);
    topPaneForm->addRow(tr("Schema"), sourceSchemaComboBox);

    if(needsTargetConnection()){
        targetConnection=new ConnectionSelectorWidget();
        connect(targetConnection, SIGNAL(connectionEstablished(DbConnection*)), this, SLOT(targetConnectionEstablished(DbConnection*)));
        topPaneForm->addRow(tr("Target connection"), targetConnection);

        targetSchemaComboBox=new DbItemListComboBox("", "user", true);
        connect(targetSchemaComboBox, SIGNAL(loadingCompleted()), this, SLOT(targetSchemaListLoaded()));
        topPaneForm->addRow(tr("Target schema"), targetSchemaComboBox);

        //addModuleSpecificControls(connectionOptionsLayout);
    }else{
        targetConnection=0;
        targetSchemaComboBox=0;
    }

    layout->addLayout(topPaneForm);
}

void DbObjectComparerCompareTab::createItemsTable(QBoxLayout *layout)
{
    schemaObjectsPanel=new DbTreeViewPanel();
    layout->addWidget(schemaObjectsPanel);
    layout->setStretchFactor(schemaObjectsPanel, 1);

    connect(schemaObjectsPanel, SIGNAL(busyStateChanged(bool)), this, SIGNAL(busyStateChanged(bool)));
}

void DbObjectComparerCompareTab::targetConnectionEstablished(DbConnection *db)
{
    WidgetHelper::setComboBoxText(targetSchemaComboBox, db->getUsername());

    targetSchemaComboBox->loadItems(this, "get_schema_list");

    emit connectionEstablished(db);
}

DbConnection *DbObjectComparerCompareTab::getDb() const
{
    return targetConnection->getDb();
}
QObject *DbObjectComparerCompareTab::getQueryEndMonitor()
{
    return this->parentQueryEndMonitor;
}

QString DbObjectComparerCompareTab::getSourceSchemaName() const
{
    return sourceSchemaComboBox->currentText().toUpper().trimmed();
}

QString DbObjectComparerCompareTab::getTargetSchemaName() const
{
    return targetSchemaComboBox!=0 ? targetSchemaComboBox->currentText().toUpper().trimmed() : "";
}

DbTreeModel *DbObjectComparerCompareTab::getObjectsModel() const
{
    return schemaObjectsPanel->tree()->getModel();
}

void DbObjectComparerCompareTab::beforeCompare()
{

}

bool DbObjectComparerCompareTab::nestOptionsTab() const
{
    return true;
}

bool DbObjectComparerCompareTab::needsTargetConnection() const
{
    return true;
}

void DbObjectComparerCompareTab::addToBottomPaneTab(QWidget *widget, const QString &title)
{
    Q_ASSERT(bottomPaneTab);

    bottomPaneTab->addTab(widget, title);
}

void DbObjectComparerCompareTab::setInitialSchemaAndObjectNames(const QString &initialSchemaName, const QString &initialObjectName)
{
    this->initialSchemaName = initialSchemaName;
    this->initialObjectName = initialObjectName;
}

void DbObjectComparerCompareTab::beforeEnqueueQuery()
{
    Q_ASSERT(queryScheduler);
    queryScheduler->increaseRefCount();
}

void DbObjectComparerCompareTab::targetSchemaListLoaded()
{
}

DbTreeModel::DbTreeNodeTypes DbObjectComparerCompareTab::getNodeTypesToDisplay() const
{
    DbTreeModel::DbTreeNodeTypes nodeTypesToDisplay;
    nodeTypesToDisplay.append(DbTreeModel::DbTreeNodeTypes() <<
                                             DbTreeModel::Tables <<
                                             DbTreeModel::Views <<
                                             DbTreeModel::Packages <<
                                             DbTreeModel::Procedures <<
                                             DbTreeModel::Functions <<
                                             DbTreeModel::Sequences <<
                                             DbTreeModel::Types <<
                                             DbTreeModel::Synonyms);
    return nodeTypesToDisplay;
}

DbTreeModel::DbTreeNodeTypes DbObjectComparerCompareTab::getCheckableNodeTypes() const
{
    DbTreeModel::DbTreeNodeTypes checkableNodeTypes;
    checkableNodeTypes.append(getNodeTypesToDisplay());
    checkableNodeTypes.append(DbTreeModel::DbTreeNodeTypes() <<
                              DbTreeModel::Table <<
                              DbTreeModel::View <<
                              DbTreeModel::Package <<
                              DbTreeModel::Procedure <<
                              DbTreeModel::Function <<
                              DbTreeModel::Sequence <<
                              DbTreeModel::Type <<
                              DbTreeModel::Synonym);
    return checkableNodeTypes;
}

void DbObjectComparerCompareTab::loadSchemaObjects()
{
    QString sourceSchema=sourceSchemaComboBox->lineEdit()->text().trimmed().toUpper();
    DbTreeView *schemaObjectsTree = schemaObjectsPanel->tree();

    if(sourceSchema.isEmpty() || sourceSchema==schemaObjectsTree->getDefaultSchemaName()){
        return;
    }

    schemaObjectsTree->setQueryScheduler(this->queryScheduler, sourceSchema, uiManager, false);
    connect(schemaObjectsTree->getModel(), SIGNAL(childrenPopulated(QModelIndex)), this, SLOT(treeChildrenPopulated(QModelIndex)));

    schemaObjectsTree->getModel()->enableCheckboxes(true);

    schemaObjectsTree->getModel()->setCheckableItemTypes(getCheckableNodeTypes());

    schemaObjectsTree->getModel()->setupInitialItems(getNodeTypesToDisplay());

    connect(schemaObjectsTree->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(currentTreeItemChanged(QModelIndex,QModelIndex)));
}

void DbObjectComparerCompareTab::sourceSchemaNameChanged()
{
    emit sourceSchemaChanged(sourceSchemaComboBox->lineEdit()->text().trimmed().toUpper());
}
