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

    QString getLastExplainPlanStatementId() const {return this->lastExpPlanStatementId;}
signals:
    void queryDone(const QueryResult &result);
    void message(const QString &msg);

private:
    IQueryScheduler *queryScheduler;
    QToolBar *toolbar;
    QAction *progressBarAction;

    MultiEditorWidget *multiEditor;

    void emitMessage(const QString &msg);
    QList<Param *> promptForBindParams(const QStringList &bindParams, const QList<BindParamInfo::BindParamType> &suggestedParamTypes);
    void saveBindParams(const QList<Param *> &params);

    QHash<QString, BindParamInfo *> paramHistory;

    QString lastExpPlanStatementId;
private slots:
    void executeQuery(ExecuteMode executeMode=ExecuteQuery);
    void executeExplainPlan();
    void queryCompleted(const QueryResult &result);

};

#endif // WORKSHEETQUERYPANE_H
