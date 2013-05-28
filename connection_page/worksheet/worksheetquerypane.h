#ifndef WORKSHEETQUERYPANE_H
#define WORKSHEETQUERYPANE_H

#include <QWidget>
#include "connectivity/queryresult.h"
#include "connectivity/ociexception.h"
#include "connectivity/sequentialqueryrunner.h"
#include "codeeditor/codeeditor.h"
#include "widgets/codeeditorandsearchpanewidget.h"
#include "beans/bindparaminfo.h"

class WorksheetCodeEditor;
class QToolButton;
class DbConnection;
class QToolBar;
class QLabel;
class IQueryScheduler;
class MultiEditorWidget;
class Param;
class BindParamInfo;

class WorksheetQueryPane : public QWidget
{
    Q_OBJECT
public:
    enum ExecuteMode
    {
        ExecuteQuery,
        ExecuteExplainPlan
    };

    explicit WorksheetQueryPane(QWidget *parent = 0);

    virtual ~WorksheetQueryPane();

    void setContents(const QString &contents);
    QString getContents() const;

    void insertText(const QString &text);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    void showSearchWidget();
    void findNext();
    void findPrevious();

    CodeEditorAndSearchPaneWidget *currentEditor() const;

    void focusAvailable();

    void setAutotraceEnabled(bool enabled);
    bool isAutotraceEnabled() const;

    QString getCurrentQuery() const {return this->currentQuery;}

    QString getLastExplainPlanStatementId() const {return this->lastExpPlanStatementId;}

    bool isInScriptMode() const;

    bool isModified() const;
    void setModified(bool modified);
signals:
    void queryDone(const QueryResult &result);
    void message(const QString &msg);
    void autotraceTriggered(bool checked);

    void scriptModeStarted();
    void scriptModeCompleted();

    void modificationChanged(bool changed);

protected:
    virtual void timerEvent(QTimerEvent *event);

private:
    IQueryScheduler *queryScheduler;
    QToolBar *toolbar;
    QAction *autotraceAction;

    QAction *progressBarAction;
    QLabel *timerLabel;
    QTime progressTimer;
    int progressTimerId;
    QAction *stopProgressAction;

    MultiEditorWidget *multiEditor;

    void emitMessage(const QString &msg);
    QList<Param *> promptForBindParams(const QStringList &bindParams, const QList<BindParamInfo::BindParamType> &suggestedParamTypes);
    void saveBindParams(const QList<Param *> &params);

    bool canExecute();
    void handleQueryCompleted(const QueryResult &result, int queryStartPos);

    QHash<QString, BindParamInfo *> paramHistory;

    QString currentQuery;
    QTextCursor currentQueryCursor;
    QString lastExpPlanStatementId;

    QString getExplainPlanPrefix() const;
    QueryResult highlightError(const QueryResult &result, int queryStartPos, bool append=false);

    void setInProgress(bool progress);
    void resetProgressLabel();
    void updateProgressLabel();

    SequentialQueryRunner sequentialRunner;
    int sequentialRunnerStartPos;
private slots:
    void executeQuery(ExecuteMode executeMode=ExecuteQuery, const QString &textToExecute = QString());
    void executeAsScript();
    void executeExplainPlan();
    void commit();
    void rollback();
    void queryCompleted(const QueryResult &result);

    void beforeExecuteSequentialQuery(const QString &query, int startPos, int endPos);
    void sequentialQueryCompleted(const QueryResult &result);
    void sequentialExecutionCompleted();

    void stopCurrentQuery();

    void autotraceTriggeredByUser(bool checked);
    void autotraceQueryCompleted(const QueryResult &result);
};

#endif // WORKSHEETQUERYPANE_H
