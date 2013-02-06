#ifndef WORKSHEETQUERYPANE_H
#define WORKSHEETQUERYPANE_H

#include <QWidget>
#include "connectivity/queryresult.h"
#include "connectivity/ociexception.h"
#include "codeeditor/codeeditor.h"
#include "widgets/codeeditorandsearchpanewidget.h"
#include "beans/bindparaminfo.h"

class WorksheetCodeEditor;
class QToolButton;
class DbConnection;
class QToolBar;
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
signals:
    void queryDone(const QueryResult &result);
    void message(const QString &msg);
    void autotraceTriggered(bool checked);

private:
    IQueryScheduler *queryScheduler;
    QToolBar *toolbar;
    QAction *progressBarAction;
    QAction *autotraceAction;

    MultiEditorWidget *multiEditor;

    void emitMessage(const QString &msg);
    QList<Param *> promptForBindParams(const QStringList &bindParams, const QList<BindParamInfo::BindParamType> &suggestedParamTypes);
    void saveBindParams(const QList<Param *> &params);

    bool canExecute();

    QHash<QString, BindParamInfo *> paramHistory;

    QString currentQuery;
    QString lastExpPlanStatementId;
private slots:
    void executeQuery(ExecuteMode executeMode=ExecuteQuery);
    void executeExplainPlan();
    void queryCompleted(const QueryResult &result);

    void autotraceTriggeredByUser(bool checked);
    void autotraceQueryCompleted(const QueryResult &result);
};

#endif // WORKSHEETQUERYPANE_H
