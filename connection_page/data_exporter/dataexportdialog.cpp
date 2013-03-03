#include "dataexportdialog.h"
#include "util/dialoghelper.h"
#include "dataexportoptionswidget.h"
#include <QtGui>

DataExportDialog::DataExportDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Export data"));

    createUi();
}

void DataExportDialog::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    exportOptionsWidget = new DataExportOptionsWidget();
    mainLayout->addWidget(exportOptionsWidget);

    QDialogButtonBox *buttonBox=DialogHelper::createButtonBox(this);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}
