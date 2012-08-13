#include "codecreator.h"
#include "metadata_loaders/code/sourceinfoloader.h"
#include "beans/sourceinfo.h"
#include "util/dbutil.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
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
    objectType(objectType),
    currentEditor(0)
{
    editMode = !objectName.isEmpty();
}

void CodeCreator::createUi()
{
    QSplitter *outerSplitter=new QSplitter(Qt::Horizontal);

    infoPanel=new InfoPanel(); //create at top to enable further use

    //create left pane
    //will use this for displaying code structure
    //splitter->addWidget(new QTreeView());

    //create right pane
    outerSplitter->addWidget(createRightPane());

    //for splitting tree/editor area and info panel
    QSplitter *bottomSplitter=new QSplitter(Qt::Vertical);
    bottomSplitter->setChildrenCollapsible(false);
    bottomSplitter->addWidget(outerSplitter);

    infoPanel->addPanel(new QPlainTextEdit("pane 1"), tr("Compile errors"), IconUtil::getIcon("help"));
    infoPanel->addPanel(new QPlainTextEdit("pane 2"), tr("Debug messages"), IconUtil::getIcon("add"));

    bottomSplitter->addWidget(infoPanel->getPanel());


    bottomSplitter->setStretchFactor(0, 2);
    bottomSplitter->setSizes(QList<int>() << 9 << 3);

    QVBoxLayout *layout=new QVBoxLayout();
    layout->setContentsMargins(0,2,0,0);
    layout->addWidget(bottomSplitter);
    layout->addWidget(infoPanel->getToolbar());

    layout->setSpacing(1);
    setLayout(layout);
}

QWidget *CodeCreator::createRightPane()
{
    QVBoxLayout *rightPaneLayout=new QVBoxLayout();

    //toolbar
    rightPaneLayout->addWidget(createToolbar());

    //create editor in splitter
    editorSplitter = new QSplitter(Qt::Horizontal);
    editorSplitter->setChildrenCollapsible(false);
    editorSplitter->addWidget(createEditor());

    rightPaneLayout->addWidget(editorSplitter);

    rightPaneLayout->setContentsMargins(0,0,0,0);
    QWidget *rightPaneWidget=new QWidget();
    rightPaneWidget->setLayout(rightPaneLayout);

    return rightPaneWidget;
}

QToolBar *CodeCreator::createToolbar()
{
    QToolBar *toolbar=new QToolBar();
    toolbar->setIconSize(QSize(16, 16));
    toolbar->addAction("test");
    WidgetHelper::addStretchToToolbar(toolbar);

    QActionGroup *editorCountGroup=new QActionGroup(this);
    for(int i=1; i<=3; ++i){
        QAction *action=new QAction(QString::number(i), editorCountGroup);
        action->setCheckable(true);
        action->setData(i);
        action->setToolTip(tr("Split editor to %1 panes").arg(i));

        if(i==1){
            action->setChecked(true);
        }

        connect(action, SIGNAL(triggered(bool)), this, SLOT(editorCountActionSelected(bool)));
    }
    toolbar->addActions(editorCountGroup->actions());

    toolbar->addSeparator();

    splitDirectionGroup=new QActionGroup(this);

    QAction *hoizontal = new QAction(IconUtil::getIcon("vertical"), tr("Align horizontally"), splitDirectionGroup);
    hoizontal->setCheckable(true);
    hoizontal->setData(Qt::Horizontal);
    hoizontal->setChecked(true);

    QAction *vertical = new QAction(IconUtil::getIcon("horizontal"), tr("Align vertically"), splitDirectionGroup);
    vertical->setCheckable(true);
    vertical->setData(Qt::Vertical);

    splitDirectionGroup->setEnabled(false);
    connect(splitDirectionGroup, SIGNAL(triggered(QAction*)), this, SLOT(editorOrientationActionSelected(QAction*)));

    toolbar->addActions(splitDirectionGroup->actions());

    return toolbar;
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

void CodeCreator::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape && infoPanel->isPanelVisible()){
        infoPanel->closePanel();
    }else{
        ConnectionPageTab::keyReleaseEvent(event);
    }
}

void CodeCreator::objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader)
{
    Q_ASSERT(objectInfo);
    Q_ASSERT(currentEditor);
    SourceInfo *sourceInfo = static_cast<SourceInfo*>(objectInfo);
    currentEditor->editor()->setPlainText(sourceInfo->source);

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

void CodeCreator::editorCountActionSelected(bool checked)
{
    if(checked){
        QAction *action=static_cast<QAction*>(sender());
        Q_ASSERT(action);

        setEditorCount(action->data().toInt());
    }
}

void CodeCreator::editorOrientationActionSelected(QAction *action)
{
    if(!action->isChecked()){
        return;
    }

    editorSplitter->setOrientation((Qt::Orientation)action->data().toInt());
}

void CodeCreator::codeEditorFocusEvent(QWidget *object, bool)
{
    currentEditor = qobject_cast<CodeEditorAndSearchPaneWidget*>(object);
    Q_ASSERT(currentEditor);
}

void CodeCreator::setEditorCount(int count)
{
    Q_ASSERT(count>0);

    int currentCount = visibleEditorCount();
    if(count>currentCount){
        for(int i=currentCount; i<=count; ++i){
            if(editors.size()>=i){
                editors.at(i-1)->show();
            }else{
                editorSplitter->addWidget(createEditor());
            }
        }
    }else if(count<currentCount){
        for(int i=currentCount; i>count; --i){
            editors.at(i-1)->hide();
            Q_ASSERT(i-2 >= 0);
            editors.at(i-2)->editor()->setFocus();
        }
    }

    splitDirectionGroup->setEnabled(count>1);
}

QWidget *CodeCreator::createEditor()
{
    CodeEditorAndSearchPaneWidget *editor=new CodeEditorAndSearchPaneWidget();
    connect(editor, SIGNAL(escapeKeyPressed()), infoPanel, SLOT(closePanel()));
    editors.append(editor);

    if(editors.size()>1){
        editor->editor()->setDocument(editors.at(0)->editor()->document()); //bind all instances to the same document (first created one)
    }

    if(editors.size()==1){
        currentEditor=editor;
    }

    connect(editor, SIGNAL(focusEvent(QWidget*,bool)), this, SLOT(codeEditorFocusEvent(QWidget*,bool)));

    return editor;
}

int CodeCreator::visibleEditorCount() const
{
    int result=0;

    for(int i=0; i<editors.size(); ++i){
        if(editors.at(i)->isVisible()){
            ++result;
        }
    }

    return result;
}
