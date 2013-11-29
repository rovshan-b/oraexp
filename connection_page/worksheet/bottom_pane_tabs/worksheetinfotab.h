#ifndef WORKSHEETINFOTAB_H
#define WORKSHEETINFOTAB_H

#include "worksheetbottompanetab.h"

class QPlainTextEdit;

class WorksheetInfoTab : public WorksheetBottomPaneTab
{
    Q_OBJECT
public:
    explicit WorksheetInfoTab(QWidget *parent = 0);

    virtual void addTabSpecificToolbarButtons();

    virtual WorksheetResultPane::WorksheetBottomPaneTabType getTabType() const;
    virtual void showQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result);

    void showMessage(const QString &msg);

    bool resultReleased() const;

private slots:
    void clear();

private:
    QPlainTextEdit *infoBox;

    QBrush blueBrush;
    QBrush redBrush;
    QBrush defaultBrush;

    void printParamValues(Statement *stmt);

    bool deletedStatement;

};

#endif // WORKSHEETINFOTAB_H
