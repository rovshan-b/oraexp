#ifndef PARTITIONINGPARAMSDIALOG_H
#define PARTITIONINGPARAMSDIALOG_H

#include <QDialog>
#include "beans/tablepartitioninginfo.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"

class TableCreatorPartitions;
class TableCreatorTabs;

class PartitioningParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PartitioningParamsDialog(QWidget *parent,
                                      DbObjectCreator::CreatorMode creatorMode,
                                      bool configureForIndex,
                                      TableCreatorTabs *tableCreator);

    TablePartitioningInfo getParams() const;
    void setParams(const TablePartitioningInfo &params, const TablePartitioningInfo &originalParams) const;

private:
    TableCreatorPartitions *partitionsWidget;

};

#endif // PARTITIONINGPARAMSDIALOG_H
