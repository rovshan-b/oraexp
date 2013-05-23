#ifndef DBITEMACTION_H
#define DBITEMACTION_H

#include <QAction>
#include "navtree/dbtreemodel.h"

class DbItemAction : public QAction
{
    Q_OBJECT
public:
    explicit DbItemAction(const QIcon &icon, const QString &text,
                          const QString &schemaName,
                          const QString &objectName,
                          const DbTreeModel::DbTreeNodeType itemType,
                          const QObject *receiver=0,
                          const char *slotName=0,
                          QObject *parent = 0);

    virtual ~DbItemAction();

    QString getSchemaName() const {return this->schemaName;}
    QString getObjectName() const {return this->objectName;}
    DbTreeModel::DbTreeNodeType getItemType() const {return this->itemType;}

    QHash<QString,QString> properties;

protected:
    QString schemaName;
    QString objectName;
    DbTreeModel::DbTreeNodeType itemType;
};

#endif // DBITEMACTION_H
