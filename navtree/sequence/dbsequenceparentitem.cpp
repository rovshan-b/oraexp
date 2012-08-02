#include "dbsequenceparentitem.h"
#include "connectivity/resultset.h"
#include "dbsequenceitem.h"

#include <QList>


DbSequenceParentItem::DbSequenceParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbSequenceParentItem::iconName() const
{
    return "sequence";
}

QList<DbTreeItem*> DbSequenceParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_sequence_list"),
                                   new Param(":owner", schemaName()));
}

DbTreeItem *DbSequenceParentItem::createNodeFromRecord(Resultset *rs)
{
    QString sequenceName=rs->getString(1);
    QString minValue=rs->getNumberAsString(2);
    QString maxValue=rs->getNumberAsString(3);
    QString cycle=rs->getString(4);
    QString cacheSize=rs->getNumberAsString(5);
    QString lastNumber=rs->getNumberAsString(6);

    DbSequenceItem *col = new DbSequenceItem(sequenceName, sequenceName, this->getModel(), this);
    QString tooltip=QObject::tr("Min value: %1\n"
                            "Max value: %2\n"
                            "Cycle: %3\n"
                            "Cache size: %4\n"
                            "Last number: %5").
                             arg(minValue).
                             arg(maxValue).
                             arg(cycle).
                             arg(cacheSize).
                             arg(lastNumber);

    col->setTooltip(tooltip);

    return col;
}
