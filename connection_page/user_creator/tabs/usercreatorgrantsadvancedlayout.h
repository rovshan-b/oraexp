#ifndef USERCREATORGRANTSADVANCEDLAYOUT_H
#define USERCREATORGRANTSADVANCEDLAYOUT_H

#include <QWidget>
#include <QModelIndex>

#include "beans/objectgrantinfo.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"

class IQueryScheduler;
class QTabWidget;
class DataTable;
class PrivGrantInfo;
class UserInfo;
class TablespaceQuotaInfo;
class IndexBasedComboBoxDelegate;
class DataTableAndToolBarWidget;
class UserRolesEditorTable;
class UserSysPrivsEditorTable;
class TablespaceQuotasEditorTable;
class ObjectGrantsEditorTable;

class UserCreatorGrantsAdvancedLayout : public QWidget
{
    Q_OBJECT
public:
    explicit UserCreatorGrantsAdvancedLayout(DbObjectCreator::CreatorMode creatorMode, QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setUserInfo(UserInfo *userInfo);

    DataTableAndToolBarWidget *getRolesTable() const;
    DataTableAndToolBarWidget *getSysPrivsTable() const;
    DataTableAndToolBarWidget *getQuotasTable() const;
    DataTableAndToolBarWidget *getObjectPrivsTable() const;

    QList<PrivGrantInfo> getUserRoles() const;
    QList<PrivGrantInfo> getUserSysPrivs() const;
    QList<TablespaceQuotaInfo> getUserQuotas() const;
    QList<ObjectGrantInfo> getUserObjectPrivs(const QString &username) const;

    void removeIncorrectRows();
signals:
    void ddlChanged();

private:
    QTabWidget *tab;
    IQueryScheduler *queryScheduler;

    UserRolesEditorTable *rolesTable;
    UserSysPrivsEditorTable *sysPrivsTable;
    TablespaceQuotasEditorTable *quotasTable;
    ObjectGrantsEditorTable *objectPrivsTable;
};

#endif // USERCREATORGRANTSADVANCEDLAYOUT_H
