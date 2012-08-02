#ifndef PARTITIONSMODEL_H
#define PARTITIONSMODEL_H

#include "genericeditabletablemodel.h"
#include "beans/partitioninfo.h"
#include "enums.h"

class PartitionsModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit PartitionsModel(const QStringList &headerTitles, OraExp::PartitionPart configureFor, QObject *parent);

    QString isRowDataCorrect(int rowIx) const;
    PartitionInfo itemInfoFromModelRow(int rowIx) const;
    void setCurrentPartitionType(OraExp::PartitionType currentPartitionType){this->currentPartitionType=currentPartitionType;}

    int PartitionNameColIx;
    int SubpartitionNameColIx;
    int ValuesColIx;
    int TablespaceColIx;

private:
    OraExp::PartitionPart configureFor;
    OraExp::PartitionType currentPartitionType;

};

#endif // PARTITIONSMODEL_H
