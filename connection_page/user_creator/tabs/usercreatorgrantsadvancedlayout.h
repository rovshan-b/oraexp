#ifndef USERCREATORGRANTSADVANCEDLAYOUT_H
#define USERCREATORGRANTSADVANCEDLAYOUT_H

#include <QWidget>

class IQueryScheduler;
class QTabWidget;
class DataTableAndToolBarWidget;

class UserCreatorGrantsAdvancedLayout : public QWidget
{
    Q_OBJECT
public:
    explicit UserCreatorGrantsAdvancedLayout(QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);
    
private:
    QTabWidget *tab;
    IQueryScheduler *queryScheduler;

    DataTableAndToolBarWidget *rolesTable;
    DataTableAndToolBarWidget *privTable;
    DataTableAndToolBarWidget *quotasTable;
    DataTableAndToolBarWidget *objectPrivTable;

    void customizeRolesTable();
    
};

#endif // USERCREATORGRANTSADVANCEDLAYOUT_H
