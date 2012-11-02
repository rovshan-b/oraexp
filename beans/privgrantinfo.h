#ifndef ROLEGRANTINFO_H
#define ROLEGRANTINFO_H

#include "defines.h"
#include "connectivity/fetchresult.h"
#include <QString>
#include <QMetaType>

class PrivGrantInfo
{
public:
    PrivGrantInfo();

    bool operator==(const PrivGrantInfo &other) const;

    int objId;
    QString name;
    bool isGrantable;
    bool isDefault;

    bool markedForDeletion;
    bool dropped;

    QString generateDdl(const QString &username) const;
    QString generateDropDdl(const QString &username) const;
    QList< NameQueryPair > generateDiffDdl(const PrivGrantInfo &other,
                                           const QString &username, const QString &taskNamePrefix) const;
    static QList< NameQueryPair > generateListDiffDdl(const QList<PrivGrantInfo> *currents,
                                                      const QList<PrivGrantInfo> *others,
                                                      const QString &username,
                                                      const QString &taskNamePrefix);
    static QStringList getDefaults(const QList<PrivGrantInfo> *list);
    bool needsRecreation(const PrivGrantInfo &other) const;

    static PrivGrantInfo fromFetchResult(const FetchResult &result);
};

Q_DECLARE_METATYPE(PrivGrantInfo)

Q_DECLARE_METATYPE(QList<PrivGrantInfo>)

#endif // ROLEGRANTINFO_H
