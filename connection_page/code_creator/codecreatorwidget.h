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
                         DbUiManager *uiManager,
                         QWidget *parent = 0);

    virtual void createUi();
    virtual void setConnection(DbConnection *db);


    virtual bool canFind() const {return true;}
    virtual void showSearchWidget(){currentEditor->showSearchPane();}
    virtual void findNext() {currentEditor->findNext();}
    virtual void findPrevious() {currentEditor->findPrevious();}

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
    void compileObject(bool forDebug=false);
    void compileObjectForDebug();

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

    void startProgress();
    void stopProgress();
    
};

#endif // CODECREATORWIDGET_H
