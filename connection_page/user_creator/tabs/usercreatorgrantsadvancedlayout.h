#ifndef USERCREATORGRANTSADVANCEDLAYOUT_H
#define USERCREATORGRANTSADVANCEDLAYOUT_H

#include <QWidget>
#include <QModelIndex>

class IQueryScheduler;
class QTabWidget;
class DataTableAndToolBarWidget;
class DataTable;
class PrivGrantInfo;
class UserInfo;

class UserCreatorGrantsAdvancedLayout : public QWidget
{
    Q_OBJECT
public:
    explicit UserCreatorGrantsAdvancedLayout(QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler, bool editMode);

    void setUserInfo(UserInfo *userInfo);

    DataTableAndToolBarWidget *getRolesTable() const;
    DataTableAndToolBarWidget *getSysPrivsTable() const;

    QList<PrivGrantInfo> getUserRoles() const;
    QList<PrivGrantInfo> getUserSysPrivs() const;

    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &taskName);

    void removeIncorrectRows();
signals:
    void ddlChanged();

private slots:
    void rolesTableDataChanged(const QModelIndex &from, const QModelIndex &to);
    void sysPrivTableDataChanged(const QModelIndex &from, const QModelIndex &to);
    
private:
    QTabWidget *tab;
    IQueryScheduler *queryScheduler;

    DataTableAndToolBarWidget *rolesTable;
    DataTableAndToolBarWidget *sysPrivTable;
    DataTableAndToolBarWidget *quotasTable;
    DataTableAndToolBarWidget *objectPrivTable;

    void customizeRolesTable(bool editMode);
    void customizeSysPrivTable(bool editMode);

    QList<PrivGrantInfo> *originalRoleList;
    QList<PrivGrantInfo> *originalSysPrivList;

    void populateTableWithRoles();
    void populateTableWithSysPrivs();

    void roleAlterQuerySucceeded(const QString &taskName);
    void roleAlterQueryError(const QString &taskName);

    void sysPrivAlterQuerySucceeded(const QString &taskName);
    void sysPrivAlterQueryError(const QString &taskName);
    
};

#endif // USERCREATORGRANTSADVANCEDLAYOUT_H
