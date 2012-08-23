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

signals:
    void objectInfoLoaded();

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

private:
    QWidget *createRightPane();
    void createToolbar();
    void setEditorCount(int count);
    QWidget *createEditor();
    int visibleEditorCount() const;
    QString getObjectTypeName() const;

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

    QList<CodeEditorAndSearchPaneWidget*> editors;

    void compileObject();
    void startProgress();
    void stopProgress();

    bool debugMode;
    
};

#endif // CODECREATORWIDGET_H
