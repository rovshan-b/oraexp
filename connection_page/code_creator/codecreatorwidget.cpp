#include "codecreatorwidget.h"
#include "metadata_loaders/code/sourceinfoloader.h"
#include "beans/sourceinfo.h"
#include "util/dbutil.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "info_panel/infopanel.h"
#include "info_panel/panes/compilermessagespane.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include <QtGui>

CodeCreatorWidget::CodeCreatorWidget(const QString &schemaName,
                         const QString &objectName,
                         DbTreeModel::DbTreeNodeType objectType,
                         QWidget *parent) :
    QWidget(parent),
    schemaName(schemaName),
    objectName(objectName),
    objectType(objectType),
    currentEditor(0)
{
    editMode = !objectName.isEmpty();
}

void CodeCreatorWidget::createUi()
{
    QSplitter *outerSplitter=new QSplitter(Qt::Horizontal);

    infoPanel=new InfoPanel(this); //create at top to enable further use

    //create left pane
    //will use this for displaying code structure
    //splitter->addWidget(new QTreeView());

    //create right pane
    outerSplitter->addWidget(createRightPane());

    //for splitting tree/editor area and info panel
    QSplitter *bottomSplitter=new QSplitter(Qt::Vertical);
    bottomSplitter->setChildrenCollapsible(false);
    bottomSplitter->addWidget(outerSplitter);

    compilerMessagesPane=new CompilerMessagesPane();
    infoPanel->addPane(compilerMessagesPane, tr("Compiler messages"), IconUtil::getIcon("error"));

    bottomSplitter->addWidget(infoPanel->getPanel());


    bottomSplitter->setSizes(QList<int>() << 3 << 1);

    QVBoxLayout *layout=new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(bottomSplitter);
    layout->addWidget(infoPanel->getToolbar());

    layout->setSpacing(1);
    setLayout(layout);
}

void CodeCreatorWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    if(DbUtil::isPLSQLProgramUnit(this->objectType)){
        if(!this->queryScheduler->getDb()->supportsCompileTimeWarnings()){
            enableWarningsAction->setEnabled(false);
            enableWarningsAction->setToolTip(tr("Compile time warnings not supported by current version of database."));
        }
        if(!this->queryScheduler->getDb()->supportsNativeCompilation()){
            enableNativeCodeAction->setEnabled(false);
            enableNativeCodeAction->setToolTip(tr("Native compilation not supported by current version of database."));
        }
    }

    if(editMode){
        if(DbUtil::isPLSQLProgramUnit(this->objectType)){
            loadCompilationParams();
        }else{
            loadObjectInfo();
        }
    }else{
        emit objectInfoLoaded();
    }
}

QWidget *CodeCreatorWidget::createRightPane()
{
    QVBoxLayout *rightPaneLayout=new QVBoxLayout();

    //toolbar
    createToolbar();
    rightPaneLayout->addWidget(toolbar);

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

void CodeCreatorWidget::createToolbar()
{
    bool isPLSQLProgramUnit = DbUtil::isPLSQLProgramUnit(this->objectType);

    toolbar=new QToolBar();
    toolbar->setIconSize(QSize(16, 16));
    if(isPLSQLProgramUnit){
        toolbar->addAction(IconUtil::getIcon("compile_for_debug"), tr("Compile for debug"), this, SLOT(compileObjectForDebug()))->setShortcut(QKeySequence(tr("Ctrl+F9","CodeCreator|Compile for debug")));
    }

    toolbar->addAction(IconUtil::getIcon("compile"), tr("Compile"), this, SLOT(compileObjectForProduction()))->setShortcut(QKeySequence(tr("F9","CodeCreator|Compile")));

    if(isPLSQLProgramUnit){
        toolbar->addSeparator();

        enableWarningsAction=toolbar->addAction(IconUtil::getIcon("warning"), tr("Enable warnings"));
        enableWarningsAction->setCheckable(true);
        if(!editMode){
            enableWarningsAction->setChecked(true);
        }

        enableNativeCodeAction=toolbar->addAction(IconUtil::getIcon("native_compilation"), tr("Enable native compilation for non-debug mode"));
        enableNativeCodeAction->setCheckable(true);
        enableNativeCodeAction->setChecked(false);
    }

    progressBarAction = WidgetHelper::addProgressBarAction(toolbar, false);

    //add buttons for accessing editor splitting functionality
    WidgetHelper::addStretchToToolbar(toolbar);

    QActionGroup *editorCountGroup=new QActionGroup(this);
    for(int i=1; i<=3; ++i){
        QAction *action=new QAction(QString::number(i), editorCountGroup);
        action->setCheckable(true);
        action->setData(i);

        if(i==1){
            action->setChecked(true);
            action->setToolTip(tr("Single editor pane"));
        }else{
            action->setToolTip(tr("Split editor to %1 panes").arg(i));
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

    WidgetHelper::updateActionTooltips(toolbar);
}

void CodeCreatorWidget::loadObjectInfo()
{
    startProgress();

    SourceInfoLoader *metadataLoader=new SourceInfoLoader(this->queryScheduler, schemaName, objectName,
                                                          getObjectTypeName(),
                                                          schemaName, false, false, this);
    connect(metadataLoader, SIGNAL(objectInfoReady(DbObjectInfo*,MetadataLoader*)), this, SLOT(objectInfoReady(DbObjectInfo*,MetadataLoader*)));
    connect(metadataLoader, SIGNAL(loadError(QString,OciException,MetadataLoader*)), this, SLOT(loadError(QString,OciException,MetadataLoader*)));
    metadataLoader->loadObjectInfo();
}

void CodeCreatorWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape && infoPanel->isPanelVisible()){
        infoPanel->closePanel();
    }else{
        QWidget::keyReleaseEvent(event);
    }
}

void CodeCreatorWidget::objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader)
{
    Q_ASSERT(objectInfo);
    Q_ASSERT(currentEditor);
    SourceInfo *sourceInfo = static_cast<SourceInfo*>(objectInfo);
    currentEditor->editor()->setPlainText(sourceInfo->source);

    delete sourceInfo;

    loader->deleteLater();

    loadCompilerMessages();

    //setBusy(false);
}

void CodeCreatorWidget::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    QMessageBox::critical(this, tr("Failed to load source code"),
                          tr("Task name: %1\nError: %2").arg(taskName, ex.getErrorMessage()));

    loader->deleteLater();

    //setBusy(false);
}

void CodeCreatorWidget::loadCompilationParams()
{
    queryScheduler->enqueueQuery("get_plsql_object_settings",
                       QList<Param*>() <<
                       new Param(":owner", this->schemaName) <<
                       new Param(":object_name", this->objectName) <<
                       new Param(":object_type", getObjectTypeName()),
                       this,
                       "get_plsql_object_settings",
                       "compilationParamsQueryCompleted",
                       "compilationParamsFetched",
                       "compilationParamsFetchCompleted");
}

void CodeCreatorWidget::compilationParamsQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this, tr("Failed to load compilation parameters"),
                              tr("Task name: %1\nError: %2").arg(result.taskName, result.exception.getErrorMessage()));
    }
}

void CodeCreatorWidget::compilationParamsFetched(const FetchResult &fetchResult)
{
    enableWarningsAction->setChecked(fetchResult.colValue("PLSQL_WARNINGS").startsWith("ENABLE"));
    enableNativeCodeAction->setChecked(fetchResult.colValue("PLSQL_CODE_TYPE")=="NATIVE");
}

void CodeCreatorWidget::compilationParamsFetchCompleted(const QString &)
{
    loadObjectInfo();
}

void CodeCreatorWidget::editorCountActionSelected(bool checked)
{
    if(checked){
        QAction *action=static_cast<QAction*>(sender());
        Q_ASSERT(action);

        setEditorCount(action->data().toInt());
    }
}

void CodeCreatorWidget::editorOrientationActionSelected(QAction *action)
{
    if(!action->isChecked()){
        return;
    }

    editorSplitter->setOrientation((Qt::Orientation)action->data().toInt());
}

void CodeCreatorWidget::codeEditorFocusEvent(QWidget *object, bool)
{
    currentEditor = qobject_cast<CodeEditorAndSearchPaneWidget*>(object);
    Q_ASSERT(currentEditor);
}

void CodeCreatorWidget::setEditorCount(int count)
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
            if(editors.at(i-1)==currentEditor){
                Q_ASSERT(i-2 >= 0);
                editors.at(i-2)->editor()->setFocus();
            }

            editors.at(i-1)->hide();
        }
    }

    splitDirectionGroup->setEnabled(count>1);
}

QWidget *CodeCreatorWidget::createEditor()
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

int CodeCreatorWidget::visibleEditorCount() const
{
    int result=0;

    for(int i=0; i<editors.size(); ++i){
        if(editors.at(i)->isVisible()){
            ++result;
        }
    }

    return result;
}

QString CodeCreatorWidget::getObjectTypeName() const
{
    return DbUtil::getDbObjectTypeNameByNodeType(this->objectType);
}

int CodeCreatorWidget::getEnableWarnings()
{
    if(!this->queryScheduler->getDb()->supportsCompileTimeWarnings() || !DbUtil::isPLSQLProgramUnit(this->objectType)){
        return 2;
    }else if(enableWarningsAction->isChecked()){
        return 1;
    }else{
        return 0;
    }
}

int CodeCreatorWidget::getEnableNativeCode()
{
    if(!this->queryScheduler->getDb()->supportsNativeCompilation() || !DbUtil::isPLSQLProgramUnit(this->objectType)){
        return 2;
    }else if(enableNativeCodeAction->isChecked()){
        return 1;
    }else{
        return 0;
    }
}

void CodeCreatorWidget::loadCompilerMessages()
{
    queryScheduler->enqueueQuery("get_compiler_messages", QList<Param*>() <<
                       new Param(":owner", schemaName) <<
                       new Param(":object_name", objectName) <<
                       new Param(":object_type", getObjectTypeName()),
                       this,
                       "get_compiler_messages",
                       "compilationCompleted",
                       "compilationErrorFetched",
                                 "compilationErrorFirstTimeFetchCompleted");
}

void CodeCreatorWidget::submitObjectCode()
{
    compilerMessagesPane->clearCompilerMessages();

    startProgress();

    queryScheduler->enqueueQuery(QString("$%1").arg(currentEditor->editor()->toPlainText()), QList<Param*>(),
                                 this,
                                 "execute_object_code",
                                 "objectCodeExecuted");
}

void CodeCreatorWidget::compileObject()
{
    queryScheduler->enqueueQuery("compile_object", QList<Param*>() <<
                       new Param(":object_name", objectName) <<
                       new Param(":owner", schemaName) <<
                       new Param(":object_type", getObjectTypeName()) <<
                       new Param(":for_debug", this->debugMode) <<
                       new Param(":enable_warnings", getEnableWarnings()) <<
                       new Param(":native_code", getEnableNativeCode() && !this->debugMode),
                       this,
                       "compile_object",
                       "compilationCompleted",
                       "compilationErrorFetched",
                       "compilationErrorFetchCompleted",
                                 true);
}

void CodeCreatorWidget::compileObjectForProduction()
{
    this->debugMode=false;
    submitObjectCode();
}

void CodeCreatorWidget::compileObjectForDebug()
{
    this->debugMode=true;
    submitObjectCode();
}

void CodeCreatorWidget::objectCodeExecuted(const QueryResult &result)
{
    delete result.statement;

    if(result.hasError){
        compilerMessagesPane->addCompilerMessage(result.exception.getErrorRow(), result.exception.getErrorPos(),
                                                 result.exception.getErrorMessage(), "ERROR");
    }

    compileObject();
}

void CodeCreatorWidget::compilationCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), result.taskName=="compile_object" ? tr("Error compiling object") : tr("Error loading compiler messages"),
                              result.exception.getErrorMessage());

        stopProgress();
    }
}

void CodeCreatorWidget::compilationErrorFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching compiler messages"), fetchResult.exception.getErrorMessage());
        stopProgress();
        return;
    }

    compilerMessagesPane->addCompilerMessage(fetchResult.colValue("LINE",0),
                                             fetchResult.colValue("POSITION",0),
                                             fetchResult.colValue("TEXT"),
                                             fetchResult.colValue("ATTRIBUTE"));
}

void CodeCreatorWidget::compilationErrorFirstTimeFetchCompleted(const QString &)
{
    emit objectInfoLoaded();
    compilationErrorFetchCompleted("");
}

void CodeCreatorWidget::compilationErrorFetchCompleted(const QString &)
{
    stopProgress();

    if(!compilerMessagesPane->isEmpty()){
        infoPanel->setCurrentPane(compilerMessagesPane);
        compilerMessagesPane->resizeToFit();
    }else{
        infoPanel->closePane(compilerMessagesPane);
    }
}

void CodeCreatorWidget::startProgress()
{
    toolbar->setEnabled(false);
    progressBarAction->setVisible(true);
}

void CodeCreatorWidget::stopProgress()
{
    progressBarAction->setVisible(false);
    toolbar->setEnabled(true);
}
