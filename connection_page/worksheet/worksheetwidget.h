#ifndef WORKSHEETWIDGET_H
#define WORKSHEETWIDGET_H

#include <QWidget>

#include "connectivity/queryresult.h"
#include "../connectionpagetab.h"
#include "worksheetquerypane.h"

class WorksheetResultPane;
class QSplitter;

class WorksheetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorksheetWidget(DbUiManager *uiManager, QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setContents(const QString &contents);

    void showSearchWidget(){queryPane->showSearchWidget();}
    void findNext() {queryPane->findNext();}
    void findPrevious() {queryPane->findPrevious();}

    void focusAvailable();

    void setAutotraceEnabled(bool enabled);
    bool isAutotraceEnabled() const;

    static QByteArray splitterSizes;
signals:
    void autotraceTriggered(bool checked);

public slots:
    void queryCompleted(const QueryResult &result);
    void splitterMoved();
    void handleQueryPaneMessage(const QString &msg);

private:
    QSplitter *splitter;
    WorksheetQueryPane *queryPane;
    WorksheetResultPane *resultPane;

    DbUiManager *uiManager;
    IQueryScheduler *queryScheduler;

    void showResultPane();
    
};

#endif // WORKSHEETWIDGET_H
