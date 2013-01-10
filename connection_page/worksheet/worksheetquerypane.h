#ifndef WORKSHEETQUERYPANE_H
#define WORKSHEETQUERYPANE_H

#include <QWidget>
#include "connectivity/queryresult.h"
#include "connectivity/ociexception.h"
#include "codeeditor/codeeditor.h"
#include "widgets/codeeditorandsearchpanewidget.h"

class WorksheetCodeEditor;
class QToolButton;
class DbConnection;
class QToolBar;
class IQueryScheduler;
class MultiEditorWidget;
class Param;

class WorksheetQueryPane : public QWidget
{
    Q_OBJECT
public:
    explicit WorksheetQueryPane(QWidget *parent = 0);

    void setContents(const QString &contents);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    void showSearchWidget();
    void findNext();
    void findPrevious();

    CodeEditorAndSearchPaneWidget *currentEditor() const;

    void focusAvailable();
signals:
    void queryDone(const QueryResult &result);
    void message(const QString &msg);

private:
    IQueryScheduler *queryScheduler;
    QToolBar *toolbar;
    QAction *progressBarAction;

    MultiEditorWidget *multiEditor;

    void emitMessage(const QString &msg);
    QList<Param *> promptForBindParams(const QStringList &bindParams);

private slots:
    void executeQuery();
    void queryCompleted(const QueryResult &result);

};

#endif // WORKSHEETQUERYPANE_H
