#ifndef WORKSHEETBOTTOMPANETAB_H
#define WORKSHEETBOTTOMPANETAB_H

#include <QWidget>
#include "../worksheetresultpane.h"

class IQueryScheduler;
class QToolBar;

class WorksheetBottomPaneTab : public QWidget
{
    Q_OBJECT
public:
    explicit WorksheetBottomPaneTab(QWidget *parent = 0);

    virtual WorksheetResultPane::WorksheetBottomPaneTabType getTabType() const = 0;
    virtual void showQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result) = 0;

    bool isPinned() const;

public slots:
    void setPinned(bool pinned);

protected:
    QToolBar *toolbar;
    QAction *progressBarAction;

    void createToolbar();

private:
    bool pinned;

};

#endif // WORKSHEETBOTTOMPANETAB_H
