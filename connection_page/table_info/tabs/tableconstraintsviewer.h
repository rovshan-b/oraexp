#ifndef TABLECONSTRAINTSVIEWER_H
#define TABLECONSTRAINTSVIEWER_H

#include "../tableinfogenerictab.h"

class TableConstraintsViewer : public TableInfoGenericTab
{
    Q_OBJECT
public:
    explicit TableConstraintsViewer(QWidget *parent = 0);

    virtual void createMainWidget(QLayout *);
};

#endif // TABLECONSTRAINTSVIEWER_H
