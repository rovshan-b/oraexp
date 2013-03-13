#ifndef DBTREEVIEWPANEL_H
#define DBTREEVIEWPANEL_H

#include <QWidget>
#include "dbtreeview.h"

class DbTreeView;
class QPushButton;
class QBoxLayout;

class DbTreeViewPanel : public QWidget
{
    Q_OBJECT
public:
    explicit DbTreeViewPanel(QWidget *parent = 0);

    DbTreeView *tree() const {return this->dbTree;}
    
private slots:
    void selectAll();
    void selectNone();
    void selectLinkedTables();

    void treeReady();

private:
    DbTreeView *dbTree;
    QPushButton *btnSelectAll;
    QPushButton *btnSelectNone;
    QPushButton *btnSelectLinkedTables;

    void createActionButtons(QBoxLayout *layout);
    
};

#endif // DBTREEVIEWPANEL_H
