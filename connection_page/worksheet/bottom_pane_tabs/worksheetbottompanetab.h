#ifndef WORKSHEETBOTTOMPANETAB_H
#define WORKSHEETBOTTOMPANETAB_H

#include <QWidget>
#include "../worksheetresultpane.h"

class IQueryScheduler;

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

private:
    bool pinned;

};

#endif // WORKSHEETBOTTOMPANETAB_H
