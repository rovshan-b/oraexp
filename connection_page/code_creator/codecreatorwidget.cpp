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
#include "errors.h"
#include <QtGui>

CodeCreatorWidget::CodeCreatorWidget(const QString &schemaName,
                         const QString &objectName,
                         DbTreeModel::DbTreeNodeType objectType,
                         QWidget *parent) :
    QWidget(parent),
    schemaName(schemaName),
    objectName(objectName),
    objectType(objectType),
    hasSpecBodySwitcher(false)
{
    editMode = !objectName.isEmpty();
}

void CodeCreatorWidget::createUi()
{
    QSplitter *outerSplitter=new QSplitter(Qt::Horizontal);

    //create at top to enable further use
    multiEditor=new MultiEditorWidget();
    infoPanel=new InfoPanel(this);

    //create left pane
    //will use this for displaying code structure
    //splitter->addWidget(new QTreeView());

    //create right pane
    outerSplitter->addWidget(createRightPane());
    connect(multiEditor, SIGNAL(escapeKeyPressed()), infoPanel, SLOT(closePanel()));

    //for splitting tree/editor area and info panel
    QSplitter *bottomSplitter=new QSplitter(Qt::Vertical);
    bottomSplitter->setChildrenCollapsible(false);
    bottomSplitter->addWidget(outerSplitter);

    compilerMessagesPane=new CompilerMessagesPane();
    infoPanel->addPane(compilerMessagesPane, tr("Compiler messages"), IconUtil::getIcon("error"));
    connect(compilerMessagesPane, SIGNAL(activated(int,int,QString)), this, SLOT(compilerMessageActivated(int,int,QString)));
    infoPanel->addInfoWidget(multiEditor->createInfoLabel());

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

void CodeCreatorWidget::setHasSpecBodySwitcher(bool hasSpecBodySwitcher, bool isSpec)
{
    this->hasSpecBodySwitcher=hasSpecBodySwitcher;
    this->isSpec=isSpec;
}

void CodeCreatorWidget::focusAvailable()
{
    currentEditor()->editor()->setFocus();
}

QWidget *CodeCreatorWidget::createRightPane()
{
    QVBoxLayout *rightPaneLayout=new QVBoxLayout();

    //toolbar
    createToolbar();
    rightPaneLayout->addWidget(toolbar);

    //add editor to splitter
    rightPaneLayout->addWidget(multiEditor);

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

    addSpecBodySwitcher();

    progressBarAction = WidgetHelper::addProgressBarAction(toolbar, false);

    //add buttons for accessing editor splitting functionality
    WidgetHelper::addStretchToToolbar(toolbar);

    multiEditor->addSplittingActions(toolbar);

    WidgetHelper::updateActionTooltips(toolbar);
}

void CodeCreatorWidget::addSpecBodySwitcher()
{
    if(!hasSpecBodySwitcher){
        return;
    }

    toolbar->addSeparator();

    QAction *specButton = toolbar->addAction(tr("Spec"));
    specButton->setCheckable(true);
    QAction *bodyButton = toolbar->addAction(tr("Body"));
    bodyButton->setCheckable(true);

    if(isSpec){
        specButton->setChecked(true);
        bodyButton->setShortcut(QKeySequence(tr("F8","CodeCreator|Toggle Spec/Body")));
    }else{
        bodyButton->setChecked(true);
        specButton->setShortcut(QKeySequence(tr("F8","CodeCreator|Toggle Spec/Body")));
    }

    specBodySwitcherGroup=new QActionGroup(this);
    specBodySwitcherGroup->addAction(specButton);
    specBodySwitcherGroup->addAction(bodyButton);

    connect(specBodySwitcherGroup, SIGNAL(triggered(QAction*)), this, SLOT(specBodySwitcherClicked(QAction*)));
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
    Q_ASSERT(currentEditor());

    SourceInfo *sourceInfo = static_cast<SourceInfo*>(objectInfo);
    currentEditor()->setInitialText(sourceInfo->source);

    delete sourceInfo;

    loader->deleteLater();

    loadCompilerMessages();

    //setBusy(false);
}

void CodeCreatorWidget::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    if(!isSpec && ex.getErrorCode()==ERR_OBJECT_NOT_FOUND){

        currentEditor()->setInitialText(tr("--Body does not exist or you do not have permission to view it."));
        emit objectInfoLoaded();

    }else{
        QMessageBox::critical(this, tr("Failed to load source code"),
                          tr("Task name: %1\nError: %2").arg(taskName, ex.getErrorMessage()));
    }

    loader->deleteLater();

    stopProgress();

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

QString CodeCreatorWidget::getObjectTypeName() const
{
    return DbUtil::getDbObjectTypeNameByNodeType(this->objectType);
}

void CodeCreatorWidget::setReadOnly(bool readOnly)
{
    multiEditor->setReadOnly(readOnly);
}

CodeEditorAndSearchPaneWidget *CodeCreatorWidget::currentEditor() const
{
    return multiEditor->getCurrentEditor();
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

    queryScheduler->enqueueQuery(QString("$%1").arg(currentEditor()->editor()->toPlainText()), QList<Param*>(),
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
        if(result.taskName=="compile_object"){
            compilerMessagesPane->addCompilerMessage(result.exception.getErrorRow(), result.exception.getErrorPos(),
                                                     QString("%1: %2").arg(tr("Error compiling object"), result.exception.getErrorMessage()),
                                                     "ERROR");
            compilationErrorFetchCompleted("");
        }else{
            QMessageBox::critical(this->window(), tr("Error loading compiler messages"),
                                  result.exception.getErrorMessage());
            stopProgress();
        }
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

void CodeCreatorWidget::specBodySwitcherClicked(QAction *)
{
    if(isSpec){
        specBodySwitcherGroup->actions().at(0)->setChecked(true);
    }else{
        specBodySwitcherGroup->actions().at(1)->setChecked(true);
    }

    emit specBodySwitchRequested();
}

void CodeCreatorWidget::compilerMessageActivated(int line, int position, const QString &)
{
    //oracle reports 1 based positions
    currentEditor()->editor()->showLinePosition(line-1, position-1);
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
    setReadOnly(true);
    progressBarAction->setVisible(true);
}

void CodeCreatorWidget::stopProgress()
{
    progressBarAction->setVisible(false);
    setReadOnly(false);
    toolbar->setEnabled(true);
}