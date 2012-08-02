#include "indexorganizedtablepropertiesdialog.h"
#include "widgets/indexorganizedtablepropertieswidget.h"
#include "util/dialoghelper.h"
#include <QtGui>

IndexOrganizedTablePropertiesDialog::IndexOrganizedTablePropertiesDialog(IQueryScheduler *queryScheduler,
                                                                         IStringListRetriever *columnListRetriever,
                                                                         bool isEditMode,
                                                                         QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout(this);

    propertiesWidget=new IndexOrganizedTablePropertiesWidget(queryScheduler, columnListRetriever, isEditMode);
    mainLayout->addWidget(propertiesWidget);

    mainLayout->addWidget(DialogHelper::createButtonBox(this));

    setLayout(mainLayout);

    setWindowTitle(tr("Index organized table properties"));
}

IndexOrganizedTableProperties IndexOrganizedTablePropertiesDialog::getAttributes() const
{
    return propertiesWidget->getAttributes();
}

void IndexOrganizedTablePropertiesDialog::setAttributes(const IndexOrganizedTableProperties &attributes,
                                                        const IndexOrganizedTableProperties &originalAttributes) const
{
    propertiesWidget->setAttributes(attributes, originalAttributes);
}
