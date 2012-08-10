#include "codecreator.h"
#include "widgets/codeeditorandsearchpanewidget.h"
#include "metadata_loaders/code/sourceinfoloader.h"
#include "beans/sourceinfo.h"
#include "util/dbutil.h"
#include "util/iconutil.h"
#include "widgets/infopanel.h"
#include <QtGui>

CodeCreator::CodeCreator(const QString &schemaName,
                         const QString &objectName,
                         DbTreeModel::DbTreeNodeType objectType,
                         DbUiManager *uiManager,
                         QWidget *parent) :
    ConnectionPageTab(uiManager, parent),
    schemaName(schemaName),
    objectName(objectName),
    objectType(objectType)
{
    editMode = !objectName.isEmpty();
}

void CodeCreator::createUi()
{
    QSplitter *topSplitter=new QSplitter(Qt::Horizontal);

    //create left pane
    //will use this for displaying code structure
    //splitter->addWidget(new QTreeView());

    //create right pane
    topSplitter->addWidget(createRightPane());

    //for splitting tree/editor area and info panel
    QSplitter *bottomSplitter=new QSplitter(Qt::Vertical);
    bottomSplitter->setChildrenCollapsible(false);
    bottomSplitter->addWidget(topSplitter);

    InfoPanel *infoPanel=new InfoPanel();
    infoPanel->addPanel(new QPlainTextEdit("pane 1"), tr("Compile errors"), IconUtil::getIcon("help"));
    infoPanel->addPanel(new QPlainTextEdit("pane 2"), tr("Debug messages"), IconUtil::getIcon("add"));
    infoPanel->setCurrentIndex(0);

    bottomSplitter->addWidget(infoPanel);


    bottomSplitter->setStretchFactor(0, 2);

    QVBoxLayout *layout=new QVBoxLayout();
    layout->setContentsMargins(0,2,0,2);
    layout->addWidget(bottomSplitter);

    setLayout(layout);
}

QWidget *CodeCreator::createRightPane()
{
    QVBoxLayout *rightPaneLayout=new QVBoxLayout();

    //toolbar
    QToolBar *toolbar=new QToolBar();
    toolbar->addAction("test");
    rightPaneLayout->addWidget(toolbar);

    //editor
    editor=new CodeEditorAndSearchPaneWidget();
    rightPaneLayout->addWidget(editor);

    rightPaneLayout->setContentsMargins(0,0,0,0);
    QWidget *rightPaneWidget=new QWidget();
    rightPaneWidget->setLayout(rightPaneLayout);

    return rightPaneWidget;
}

void CodeCreator::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    if(editMode){
        SourceInfoLoader *metadataLoader=new SourceInfoLoader(this, schemaName, objectName,
                                                              DbUtil::getDbObjectTypeNameByNodeType(objectType),
                                                              schemaName, false, this);
        connect(metadataLoader, SIGNAL(objectInfoReady(DbObjectInfo*,MetadataLoader*)), this, SLOT(objectInfoReady(DbObjectInfo*,MetadataLoader*)));
        connect(metadataLoader, SIGNAL(loadError(QString,OciException,MetadataLoader*)), this, SLOT(loadError(QString,OciException,MetadataLoader*)));
        metadataLoader->loadObjectInfo();
    }else{
        setBusy(false);
    }
}

void CodeCreator::objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader)
{
    Q_ASSERT(objectInfo);
    SourceInfo *sourceInfo = static_cast<SourceInfo*>(objectInfo);
    editor->editor()->setPlainText(sourceInfo->source);

    delete sourceInfo;

    loader->deleteLater();

    setBusy(false);
}

void CodeCreator::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    QMessageBox::critical(this, tr("Failed to load source code"),
                          tr("Task name: %1\nError: %2").arg(taskName, ex.getErrorMessage()));

    loader->deleteLater();

    setBusy(false);
}
