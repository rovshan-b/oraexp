#ifndef USERCREATORGRANTSADVANCEDLAYOUT_H
#define USERCREATORGRANTSADVANCEDLAYOUT_H

#include <QWidget>

class IQueryScheduler;
class QTabWidget;
class DataTableAndToolBarWidget;
class DataTable;

class UserCreatorGrantsAdvancedLayout : public QWidget
{
    Q_OBJECT
public:
    explicit UserCreatorGrantsAdvancedLayout(QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    DataTableAndToolBarWidget *getRolesTable() const;
    DataTableAndToolBarWidget *getSysPrivsTable() const;
    
private:
    QTabWidget *tab;
    IQueryScheduler *queryScheduler;

    DataTableAndToolBarWidget *rolesTable;
    DataTableAndToolBarWidget *sysPrivTable;
    DataTableAndToolBarWidget *quotasTable;
    DataTableAndToolBarWidget *objectPrivTable;

    void customizeRolesTable();
    void customizeSysPrivTable();
    
};

#endif // USERCREATORGRANTSADVANCEDLAYOUT_H
