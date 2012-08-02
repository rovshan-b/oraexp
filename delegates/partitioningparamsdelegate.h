#ifndef PARTITIONINGPARAMSDELEGATE_H
#define PARTITIONINGPARAMSDELEGATE_H

#include "abstracttemplatedparamsdelegate.h"
#include "beans/tablepartitioninginfo.h"
#include "dialogs/partitioningparamsdialog.h"

class PartitioningParamsDelegate : public AbstractTemplatedParamsDelegate<TablePartitioningInfo, PartitioningParamsDialog>
{
public:
    PartitioningParamsDelegate(IQueryScheduler *queryScheduler,
                               bool configureForIndex,
                               TableCreatorTabs *tableCreator,
                               QObject *parent);

protected:
    virtual PartitioningParamsDialog *createEditorDialog(bool isEditMode, QWidget *parentWindow);
    QString generateDdl(const TablePartitioningInfo &params);
    QString generateDiffDdl(const TablePartitioningInfo &params, const TablePartitioningInfo &originalParams);

private:
    bool configureForIndex;
    TableCreatorTabs *tableCreator;
};

#endif // PARTITIONINGPARAMSDELEGATE_H
