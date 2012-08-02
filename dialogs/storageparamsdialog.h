#ifndef STORAGEPARAMSDIALOG_H
#define STORAGEPARAMSDIALOG_H

#include <QDialog>
#include "beans/storageparams.h"

class IQueryScheduler;
class StorageParamsWidget;

class StorageParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StorageParamsDialog(IQueryScheduler *queryScheduler, QWidget *parent,
                                 bool isEditMode,
                                 bool configureForIndex=false);

    StorageParams getParams() const;
    void setParams(const StorageParams &params, const StorageParams &) const;

private:
    StorageParamsWidget *storageParamsWidget;

};

#endif // STORAGEPARAMSDIALOG_H
