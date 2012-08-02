#ifndef INDEXSTORAGEPARAMSDELEGATE_H
#define INDEXSTORAGEPARAMSDELEGATE_H

#include "templatedparamsdelegate.h"

class IndexStorageParamsDelegate : public TemplatedParamsDelegate<StorageParams, StorageParamsDialog>
{
public:
    IndexStorageParamsDelegate(IQueryScheduler *queryScheduler, QObject *parent);

protected:
    virtual StorageParamsDialog *createEditorDialog(bool isEditMode, QWidget *parentWindow);
};

#endif // INDEXSTORAGEPARAMSDELEGATE_H
