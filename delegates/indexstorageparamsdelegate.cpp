#include "indexstorageparamsdelegate.h"

IndexStorageParamsDelegate::IndexStorageParamsDelegate(IQueryScheduler *queryScheduler, QObject *parent) :
    TemplatedParamsDelegate<StorageParams, StorageParamsDialog>(queryScheduler, parent)
{
}

StorageParamsDialog *IndexStorageParamsDelegate::createEditorDialog(bool isEditMode, QWidget *parentWindow)
{
    StorageParamsDialog *dialog=new StorageParamsDialog(queryScheduler, parentWindow, isEditMode, true);
    return dialog;
}
