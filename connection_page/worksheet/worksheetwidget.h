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
    explicit WorksheetWidget(QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setContents(const QString &contents);

    void showSearchWidget(){queryPane->showSearchWidget();}
    void findNext() {queryPane->findNext();}
    void findPrevious() {queryPane->findPrevious();}

    void focusAvailable();

    static QByteArray splitterSizes;
signals:
    
public slots:
    void queryCompleted(const QueryResult &result);
    void splitterMoved();

private:
    QSplitter *splitter;
    WorksheetQueryPane *queryPane;
    WorksheetResultPane *resultPane;

    IQueryScheduler *queryScheduler;
    
};

#endif // WORKSHEETWIDGET_H
