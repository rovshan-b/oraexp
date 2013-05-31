#include "partitioningparamsdialog.h"
#include "connection_page/table_creator/tabs/tablecreatorpartitions.h"
#include "util/dialoghelper.h"
#include "connection_page/table_creator/tablecreatortabs.h"
#include <QtGui>

PartitioningParamsDialog::PartitioningParamsDialog(QWidget *parent,
                                                   DbObjectCreator::CreatorMode creatorMode,
                                                   bool configureForIndex,
                                                   TableCreatorTabs *tableCreator) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout(this);

    partitionsWidget=new TableCreatorPartitions(tableCreator, creatorMode, configureForIndex, this);
    partitionsWidget->setQueryScheduler(tableCreator->getQueryScheduler());
    partitionsWidget->layout()->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(partitionsWidget);

    QDialogButtonBox *buttonBox = DialogHelper::createButtonBox(this, QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    setWindowTitle(tr("Partition definition"));
}

TablePartitioningInfo PartitioningParamsDialog::getParams() const
{
    return partitionsWidget->getPartitioningInfo(true);
}

void PartitioningParamsDialog::setParams(const TablePartitioningInfo &params, const TablePartitioningInfo &originalParams) const
{
    partitionsWidget->setPartitioningInfo(params, originalParams);
}
