#ifndef OBJECTGRANTINFO_H
#define OBJECTGRANTINFO_H

#include <QString>
#include <QStringList>
#include "connectivity/fetchresult.h"
#include "enums.h"
#include "defines.h"
#include "navtree/dbtreemodel.h"
#include <QMetaType>

class ObjectGrantInfo
{
public:
    ObjectGrantInfo();

    bool operator==(const ObjectGrantInfo &other) const;

    int objId;
    QString grantee;
    QString objectName;
    DbTreeModel::DbTreeNodeType objectType;
    QHash<QString,int> privileges;

    bool markedForDeletion;
    bool dropped;

    static ObjectGrantInfo fromFetchResult(const FetchResult &result, DbTreeModel::DbTreeNodeType objectType);

    QList< NameQueryPair > generateDdl() const;
    QString generateRevokeAllDdl() const;
    QList< NameQueryPair > generateDiffDdl(const ObjectGrantInfo &other) const;
    bool needsRecreation(const ObjectGrantInfo &) const {return false;}
private:
    QString generateGrantStatement(const QString &privilegeName,
                                   const QString &granteeName,
                                   int grantType,
                                   bool generateRevoke=false) const;

    static int getPrivilegeIndex(const QString &privilegeName, const QStringList &privileges);

};

Q_DECLARE_METATYPE(ObjectGrantInfo)

Q_DECLARE_METATYPE(QList<ObjectGrantInfo>)

#endif // OBJECTGRANTINFO_H
