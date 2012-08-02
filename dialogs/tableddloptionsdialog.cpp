#include "tableddloptionsdialog.h"
#include "widgets/tableddloptionswidget.h"
#include "util/dialoghelper.h"
#include <QtGui>

TableDdlOptionsDialog::TableDdlOptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout(this);

    optionsWidget=new TableDdlOptionsWidget();
    mainLayout->addWidget(optionsWidget);

    mainLayout->addWidget(DialogHelper::createButtonBox(this));

    setLayout(mainLayout);

    setWindowTitle(tr("DDL options"));
}

ObjectDdlOptions TableDdlOptionsDialog::getOptions() const
{
    return optionsWidget->getOptions();
}

void TableDdlOptionsDialog::setOptions(const ObjectDdlOptions &options)
{
    optionsWidget->setOptions(options);
}
