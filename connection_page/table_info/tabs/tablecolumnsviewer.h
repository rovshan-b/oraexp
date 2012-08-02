#ifndef TABLECOLUMNSVIEWER_H
#define TABLECOLUMNSVIEWER_H

#include "../tableinfogenerictab.h"

class TableColumnsViewer : public TableInfoGenericTab
{
    Q_OBJECT
public:
    explicit TableColumnsViewer(QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

protected:
    virtual void addSpecificToolbarButtons();

private slots:
    void detailedViewToggled(bool showDetailedView);

};

#endif // TABLECOLUMNSVIEWER_H
