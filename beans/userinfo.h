#ifndef USERINFO_H
#define USERINFO_H

#include "dbobjectinfo.h"
#include "usergeneralinfo.h"
#include "privgrantinfo.h"
#include "tablespacequotainfo.h"
#include "beans/querylistitem.h"
#include "beans/objectgrantinfo.h"

#include <QString>
#include <QList>
#include <QHash>
#include <QMetaType>

class UserInfo : public DbObjectInfo
{
public:
    UserInfo();

    UserGeneralInfo generalInfo;
    QList<PrivGrantInfo> roles;
    QList<PrivGrantInfo> sysPrivs;
    QList<TablespaceQuotaInfo> quotas;
    QList<ObjectGrantInfo> objectPrivs;

    QString generateDdl() const;
    QString generateDropDdl() const;
    QList<QueryListItem> generateDiffDdl(const UserInfo &other, const QHash<QString, QObject *> &requesters) const;
    QString generateDefaultRolesDdl(const QStringList &defaultRoles) const;
    QStringList validate(bool editMode) const;
};

Q_DECLARE_METATYPE(UserInfo)


#endif // USERINFO_H
