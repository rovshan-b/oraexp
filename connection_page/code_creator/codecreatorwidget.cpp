#include "codecreatorwidget.h"
#include "metadata_loaders/code/sourceinfoloader.h"
#include "beans/sourceinfo.h"
#include "util/dbutil.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/queryutil.h"
#include "util/codeeditorutil.h"
#include "info_panel/infopanel.h"
#include "info_panel/panes/compilermessagespane.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "widgets/specbodyswitcherwidget.h"
#include "codeeditor/autocompletehelper.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include "errors.h"
#include <QtGui>

QByteArray CodeCreatorWidget::bottomSplitterSizes;

CodeCreatorWidget::CodeCreatorWidget(const QString &schemaName,
                         const QString &objectName,
                         DbTreeModel::DbTreeNodeType objectType,
                         DbUiManager *uiManager, QWidget *parent) :
    QWidget(parent),
    schemaName(schemaName),
    objectName(objectName),
    objectType(objectType),
    uiManager(uiManager),
    hasSpecBodySwitcher(false),
    autocompleteHelper(0)
{
    editMode = !objectName.isEmpty();
}

void CodeCreatorWidget::createUi()
{
    QSplitter *outerSplitter=new QSplitter(Qt::Horizontal);

    //create at top to enable further use
    multiEditor=new MultiEditorWidget(uiManager, DbUtil::isPLSQLProgramUnit(objectType));
    infoPanel=new InfoPanel(this);

    //create left pane
    //will use this for displaying code structure
    //splitter->addWidget(new QTreeView());

    //create right pane
    outerSplitter->addWidget(createRightPane());
    connect(multiEditor, SIGNAL(escapeKeyPressed()), infoPanel, SLOT(closePanel()));
    connect(multiEditor, SIGNAL(needsCompletionList()), this, SLOT(prepareCompletionList()));

    //for splitting tree/editor area and info panel
    bottomSplitter=new QSplitter(Qt::Vertical);
    bottomSplitter->setChildrenCollapsible(false);
    bottomSplitter->addWidget(outerSplitter);

    compilerMessagesPane=new CompilerMessagesPane();
    infoPanel->addPane(compilerMessagesPane, tr("Compiler messages"), IconUtil::getIcon("error"));
    connect(compilerMessagesPane, SIGNAL(activated(int,int,QString)), this, SLOT(compilerMessageActivated(int,int,QString)));
    infoPanel->addInfoWidget(multiEditor->createInfoLabel());

    bottomSplitter->addWidget(infoPanel->getPanel());


    //bottomSplitter->setSizes(QList<int>() << 3 << 1);
    bottomSplitter->restoreState(CodeCreatorWidget::bottomSplitterSizes);

    QVBoxLayout *layout=new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(bottomSplitter);
    layout->addWidget(infoPanel->getToolbar());

    layout->setSpacing(1);
    setLayout(layout);

    connect(bottomSplitter, SIGNAL(splitterMoved(int,int)), this, SLOT(bottomSplitterMoved()));
    connect(currentEditor()->editor()->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
}

void CodeCreatorWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;
    multiEditor->setQueryScheduler(queryScheduler);

    Q_ASSERT(autocompleteHelper == 0);
    autocompleteHelper = new AutocompleteHelper(this);

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
        loadCodeTemplate();

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

bool CodeCreatorWidget::isModified() const
{
    return currentEditor()->editor()->document()->isModified();
}

void CodeCreatorWidget::setModified(bool modified)
{
    currentEditor()->editor()->document()->setModified(modified);
}

QString CodeCreatorWidget::getDefaultSaveSuffix() const
{
    return DbUtil::getSaveSuffixForItemType(this->objectType);
}

QString CodeCreatorWidget::getContents() const
{
    return currentEditor()->editor()->toPlainText();
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
        enableWarningsAction->setChecked(false);

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
    SpecBodySwitcherWidget *switcher=new SpecBodySwitcherWidget(isSpec, this);
    toolbar->addWidget(switcher);

    connect(switcher, SIGNAL(specBodySwitchRequested()), this, SIGNAL(specBodySwitchRequested()));
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

    QString source = sourceInfo->source;

    if(objectType == DbTreeModel::View){
        PlSqlParseHelper::prepareViewForEditing(source);
    }else if(objectType == DbTreeModel::Trigger){
        PlSqlParseHelper::prepareTriggerForEditing(source);
    }

    multiEditor->setInitialText(source);

    delete sourceInfo;

    loader->deleteLater();

    loadCompilerMessages();

    //setBusy(false);
}

void CodeCreatorWidget::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    if(!isSpec && ex.getErrorCode()==ERR_OBJECT_NOT_FOUND){

        multiEditor->setInitialText(tr("--Object does not exist or you do not have permission to view it."));
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
    emit stateChanged();
}

void CodeCreatorWidget::loadCodeTemplate()
{
    QString typeName = getObjectTypeName().toLower().replace(' ','_');
    QString codeTemplate = QueryUtil::getQuery(QString("code_template_%1").arg(typeName), this->queryScheduler->getDb());
    multiEditor->setInitialText(codeTemplate);
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
    }else if(enableNativeCodeAction->isChecked() && !this->debugMode){
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
                       new Param(":native_code", getEnableNativeCode()),
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
        addCompilerMessage(result.exception.getErrorRow(),
                           result.exception.getErrorPos(),
                           result.exception.getErrorCode(),
                           result.exception.getErrorMessage(),
                           "ERROR");
        stopProgress();
        infoPanel->setCurrentPane(compilerMessagesPane);
        return;
    }

    QString query = currentEditor()->editor()->toPlainText();
    QString schema, name;
    PlSqlParseHelper::findObjectName(query, &schema, &name, this->queryScheduler->getDb()->getUsername());
    if(name.isEmpty() && objectName.isEmpty()){
        QMessageBox::critical(this->window(), tr("Parse error"),
                              tr("Could not parse object name. If you know that code is correct please, submit a bug report."));
    }else{
        this->schemaName = schema;
        this->objectName = name;
    }

    setModified(false);
    compileObject();
}

void CodeCreatorWidget::compilationCompleted(const QueryResult &result)
{
    if(result.hasError){
        if(result.taskName=="compile_object"){
            addCompilerMessage(result.exception.getErrorRow(),
                               result.exception.getErrorPos(),
                               result.exception.getErrorCode(),
                               QString("%1: %2").arg(tr("Error compiling object"),
                               result.exception.getErrorMessage()),
                               "ERROR");
            compilationErrorFetchCompleted("");
        }else{
            QMessageBox::critical(this->window(), tr("Error loading compiler messages"),
                                  result.exception.getErrorMessage());
            stopProgress();
        }
    }
}

void CodeCreatorWidget::addCompilerMessage(int line, int position, int errorCode, const QString &message, const QString &attribute)
{
    if(line > 0 && compilerMessagesPane->isEmpty()){
        QTextCursor cursor = currentEditor()->editor()->textCursor();
        cursor.setPosition(0);

        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, line-1);

        if(position > 0){
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, position-1);
        }

        foreach(CodeEditorAndSearchPaneWidget *e, multiEditor->getEditors()){
            CodeEditorUtil::highlightEditorError(e->editor(), cursor.position(), errorCode, message, true);
        }
    }

    compilerMessagesPane->addCompilerMessage(line,
                                             position,
                                             message,
                                             attribute);
}

void CodeCreatorWidget::compilationErrorFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching compiler messages"), fetchResult.exception.getErrorMessage());
        stopProgress();
        return;
    }


    QString errorText = fetchResult.colValue("TEXT").trimmed();

    addCompilerMessage(fetchResult.colValue("LINE",0),
                       fetchResult.colValue("POSITION",0),
                       PlSqlParseHelper::extractPlSqlErrorCode(errorText),
                       errorText,
                       fetchResult.colValue("ATTRIBUTE"));
}

void CodeCreatorWidget::compilationErrorFirstTimeFetchCompleted(const QString &)
{
    emit objectInfoLoaded();
    compilationErrorFetchCompleted("");
}

void CodeCreatorWidget::compilerMessageActivated(int line, int position, const QString &)
{
    //oracle reports 1 based positions
    currentEditor()->editor()->showLinePosition(line-1, position-1);
}

void CodeCreatorWidget::bottomSplitterMoved()
{
    CodeCreatorWidget::bottomSplitterSizes = bottomSplitter->saveState();
}

QString CodeCreatorWidget::getCurrentFileName() const
{
    return this->currentFileName;
}

void CodeCreatorWidget::setCurrentFileName(const QString &fileName)
{
    this->currentFileName = fileName;
}

QTextDocument *CodeCreatorWidget::getTextDocument() const
{
    return currentEditor()->getTextDocument();
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

void CodeCreatorWidget::prepareCompletionList()
{
    if(!this->queryScheduler){
        return;
    }

    /*
        resolve steps (name can consist of several parts)
        check first token for following
          1. is it local variable name
          2. try to find it in all_objects (if it is a snynonym, find out target object)
          3. is it schema name

        if first token is resolved go on to resolve next parts
    */

    CodeEditor *editor = multiEditor->currentTextEditor();
    QString currentObjectName = CodeEditorUtil::getCurrentObjectName(editor);
    if(!currentObjectName.isEmpty() && currentObjectName.length() < 250){
        QStringList nameParts = PlSqlParseHelper::tokenizeName(currentObjectName);
        if(nameParts.size() > 0){
            autocompleteHelper->getChildList(nameParts, editor->textCursor().position());
        }
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
