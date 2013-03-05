#include "dataexportdialog.h"
#include "util/dialoghelper.h"
#include "util/iconutil.h"
#include <QtGui>

DataExportDialog::DataExportDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Export data"));
    setWindowIcon(IconUtil::getIcon("export"));

    createUi();
}

void DataExportDialog::accept()
{
    if(exportOptionsWidget->validate()){
        QDialog::accept();
    }
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
