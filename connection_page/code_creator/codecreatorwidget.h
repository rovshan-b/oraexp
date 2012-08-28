#ifndef CODECREATORWIDGET_H
#define CODECREATORWIDGET_H

#include "../connectionpagetab.h"
#include "navtree/dbtreemodel.h"
#include "widgets/codeeditorandsearchpanewidget.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class DbObjectInfo;
class MetadataLoader;
class CodeEditorAndSearchPaneWidget;
class InfoPanel;
class QSplitter;
class QToolBar;
class QActionGroup;
class CompilerMessagesPane;

class CodeCreatorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CodeCreatorWidget(const QString &schemaName,
                         const QString &objectName,
                         DbTreeModel::DbTreeNodeType objectType,
                         QWidget *parent = 0);

    void createUi();
    void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual bool canFind() const {return true;}
    virtual void showSearchWidget(){currentEditor->showSearchPane();}
    virtual void findNext() {currentEditor->findNext();}
    virtual void findPrevious() {currentEditor->findPrevious();}

    void setHasSpecBodySwitcher(bool hasSpecBodySwitcher, bool isSpec);

signals:
    void objectInfoLoaded();
    void specBodySwitchRequested();

protected:
    void keyReleaseEvent (QKeyEvent * event);
    
private slots:
    void loadObjectInfo();
    void objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);

    void loadCompilationParams();
    void compilationParamsQueryCompleted(const QueryResult &result);
    void compilationParamsFetched(const FetchResult &fetchResult);
    void compilationParamsFetchCompleted(const QString &);

    void editorCountActionSelected(bool checked);
    void editorOrientationActionSelected(QAction *action);
    void codeEditorFocusEvent(QWidget *object, bool focusIn);

    int getEnableWarnings();
    int getEnableNativeCode();

    void loadCompilerMessages();
    void submitObjectCode();
    void compileObjectForProduction();
    void compileObjectForDebug();

    void objectCodeExecuted(const QueryResult &result);
    void compilationCompleted(const QueryResult &result);
    void compilationErrorFetched(const FetchResult &fetchResult);
    void compilationErrorFetchCompleted(const QString &);
    void compilationErrorFirstTimeFetchCompleted(const QString &);

    void specBodySwitcherClicked(QAction *);

    void cursorPositionChanged();

    void compilerMessageActivated(int line, int position, const QString &);

private:
    QWidget *createRightPane();
    void createToolbar();
    void addSpecBodySwitcher();
    void setEditorCount(int count);
    QWidget *createEditor();
    int visibleEditorCount() const;
    QString getObjectTypeName() const;
    void setReadOnly(bool readOnly);

    QString schemaName;
    QString objectName;
    DbTreeModel::DbTreeNodeType objectType;

    bool editMode;

    IQueryScheduler *queryScheduler;

    QToolBar *toolbar;
    QAction *progressBarAction;
    QAction *enableWarningsAction;
    QAction *enableNativeCodeAction;

    QSplitter *editorSplitter;
    CodeEditorAndSearchPaneWidget *currentEditor;
    InfoPanel *infoPanel;
    CompilerMessagesPane *compilerMessagesPane;

    QActionGroup *splitDirectionGroup;
    QActionGroup *specBodySwitcherGroup;

    QList<CodeEditorAndSearchPaneWidget*> editors;

    void compileObject();
    void startProgress();
    void stopProgress();

    bool debugMode;

    bool hasSpecBodySwitcher;
    bool isSpec;

    QString infoLabelTextFormat;
    
};

#endif // CODECREATORWIDGET_H
