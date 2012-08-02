#include "storageparamsdialog.h"
#include "widgets/storageparamswidget.h"
#include "util/dialoghelper.h"
#include <QtGui>

StorageParamsDialog::StorageParamsDialog(IQueryScheduler *queryScheduler, QWidget *parent,
                                         bool isEditMode,
                                         bool configureForIndex) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout(this);

    storageParamsWidget=new StorageParamsWidget(queryScheduler, isEditMode, configureForIndex);
    mainLayout->addWidget(storageParamsWidget);

    mainLayout->addWidget(DialogHelper::createButtonBox(this));

    setLayout(mainLayout);

    setWindowTitle(tr("Physical attributes"));
}

StorageParams StorageParamsDialog::getParams() const
{
    return storageParamsWidget->getStorageParams();
}

void StorageParamsDialog::setParams(const StorageParams &params, const StorageParams &/*originalParams*/) const
{
    storageParamsWidget->setStorageParams(params);
}
