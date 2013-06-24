#include "tableddloptionsdialog.h"
#include "widgets/tabledbmsddloptionswidget.h"
#include "util/dialoghelper.h"
#include <QtGui>

TableDdlOptionsDialog::TableDdlOptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout(this);

    optionsWidget=new TableDbmsDdlOptionsWidget();
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
