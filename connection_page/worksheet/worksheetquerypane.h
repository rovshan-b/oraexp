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

private:
    IQueryScheduler *queryScheduler;
    QToolBar *toolbar;
    QAction *progressBarAction;

    MultiEditorWidget *multiEditor;

private slots:
    void executeQuery();
    void queryCompleted(const QueryResult &result);

};

#endif // WORKSHEETQUERYPANE_H
