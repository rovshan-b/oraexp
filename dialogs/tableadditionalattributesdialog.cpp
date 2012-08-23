#include "tableadditionalattributesdialog.h"
#include "widgets/tableadditionalattributeswidget.h"
#include "util/dialoghelper.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

TableAdditionalAttributesDialog::TableAdditionalAttributesDialog(IQueryScheduler *queryScheduler,
                                                                 OraExp::TableType tableType,
                                                                 bool isEditMode,
                                                                 QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout(this);

    attributesWidget=new TableAdditionalAttributesWidget(queryScheduler, tableType, isEditMode);
    mainLayout->addWidget(attributesWidget);

    mainLayout->addWidget(DialogHelper::createButtonBox(this));

    setLayout(mainLayout);

    setWindowTitle(tr("Properties"));
}

TableAdditionalAttributes TableAdditionalAttributesDialog::getAttributes() const
{
    return attributesWidget->getAttributes();
}

void TableAdditionalAttributesDialog::setAttributes(const TableAdditionalAttributes &attributes) const
{
    attributesWidget->setAttributes(attributes);
}
