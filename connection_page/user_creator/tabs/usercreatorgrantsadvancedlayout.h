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
class TablespaceQuotaInfo;
class IndexBasedComboBoxDelegate;
class UserRolesEditorTable;
class UserSysPrivsEditorTable;
class TablespaceQuotasEditorTable;

class UserCreatorGrantsAdvancedLayout : public QWidget
{
    Q_OBJECT
public:
    explicit UserCreatorGrantsAdvancedLayout(bool editMode, QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setUserInfo(UserInfo *userInfo);

    DataTableAndToolBarWidget *getRolesTable() const;
    DataTableAndToolBarWidget *getSysPrivsTable() const;
    DataTableAndToolBarWidget *getQuotasTable() const;

    QList<PrivGrantInfo> getUserRoles() const;
    QList<PrivGrantInfo> getUserSysPrivs() const;
    QList<TablespaceQuotaInfo> getUserQuotas() const;

    void removeIncorrectRows();
signals:
    void ddlChanged();

private:
    QTabWidget *tab;
    IQueryScheduler *queryScheduler;

    UserRolesEditorTable *rolesTable;
    UserSysPrivsEditorTable *sysPrivsTable;
    TablespaceQuotasEditorTable *quotasTable;
    DataTableAndToolBarWidget *objectPrivTable;
};

#endif // USERCREATORGRANTSADVANCEDLAYOUT_H
