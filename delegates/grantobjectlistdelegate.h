#ifndef GRANTOBJECTLISTDELEGATE_H
#define GRANTOBJECTLISTDELEGATE_H

#include "dbitemlistdelegate.h"

class GrantObjectListDelegate : public DbItemListDelegate
{
    Q_OBJECT
public:
    explicit GrantObjectListDelegate(IQueryScheduler *queryScheduler,
                                     int modelSchemaColumnIndex,
                                     QObject *parent);
    
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;
    
private:
    int modelSchemaColumnIndex;
};

#endif // GRANTOBJECTLISTDELEGATE_H
