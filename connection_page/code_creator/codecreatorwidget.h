#ifndef CODECREATORWIDGET_H
#define CODECREATORWIDGET_H

#include "../connectionpagetab.h"
#include "navtree/dbtreemodel.h"
#include "widgets/multieditorwidget.h"
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
class MultiEditorWidget;

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
    virtual void showSearchWidget(){currentEditor()->showSearchPane();}
    virtual void findNext() {currentEditor()->findNext();}
    virtual void findPrevious() {currentEditor()->findPrevious();}

    void setHasSpecBodySwitcher(bool hasSpecBodySwitcher, bool isSpec);

    void focusAvailable();

    bool isModified() const;
    void setModified(bool modified);

    QString getContents() const;

    virtual QString getCurrentFileName() const;
    virtual void setCurrentFileName(const QString &fileName);

    static QByteArray bottomSplitterSizes;

signals:
    void objectInfoLoaded();
    void specBodySwitchRequested();
    void modificationChanged(bool changed);

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

    void compilerMessageActivated(int line, int position, const QString &);

    void bottomSplitterMoved();

private:
    QWidget *createRightPane();
    void createToolbar();
    void addSpecBodySwitcher();
    QString getObjectTypeName() const;
    void setReadOnly(bool readOnly);

    CodeEditorAndSearchPaneWidget *currentEditor() const;

    QString schemaName;
    QString objectName;
    DbTreeModel::DbTreeNodeType objectType;

    bool editMode;

    IQueryScheduler *queryScheduler;

    QToolBar *toolbar;
    QAction *progressBarAction;
    QAction *enableWarningsAction;
    QAction *enableNativeCodeAction;

    QSplitter *bottomSplitter;

    InfoPanel *infoPanel;
    CompilerMessagesPane *compilerMessagesPane;

    void compileObject();
    void startProgress();
    void stopProgress();

    bool debugMode;

    bool hasSpecBodySwitcher;
    bool isSpec;

    bool stateRestored;

    MultiEditorWidget *multiEditor;

    QString currentFileName;

};

#endif // CODECREATORWIDGET_H
