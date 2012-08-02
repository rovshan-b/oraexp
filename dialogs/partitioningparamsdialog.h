#ifndef PARTITIONINGPARAMSDIALOG_H
#define PARTITIONINGPARAMSDIALOG_H

#include <QDialog>
#include "beans/tablepartitioninginfo.h"

class TableCreatorPartitions;
class TableCreatorTabs;

class PartitioningParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PartitioningParamsDialog(QWidget *parent,
                                      bool isEditMode,
                                      bool configureForIndex,
                                      TableCreatorTabs *tableCreator);

    TablePartitioningInfo getParams() const;
    void setParams(const TablePartitioningInfo &params, const TablePartitioningInfo &originalParams) const;

private:
    TableCreatorPartitions *partitionsWidget;

};

#endif // PARTITIONINGPARAMSDIALOG_H
