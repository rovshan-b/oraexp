#include "tablecreatelikeoptionsdialog.h"
#include "widgets/existingtableoptionswidget.h"
#include "util/dialoghelper.h"
#include "util/iconutil.h"
#include "interfaces/iqueryscheduler.h"
#include "connectivity/dbconnection.h"
#include <QtGui>

TableCreateLikeOptionsDialog::TableCreateLikeOptionsDialog(IQueryScheduler *queryScheduler, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Options to copy"));
    setWindowIcon(IconUtil::getIcon("table_copy"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->addWidget(new QLabel(tr("Select options to copy to new table")));

    optionsWidget = new ExistingTableOptionsWidget(false);
    bool flashbackSupported = queryScheduler->getDb()->supportsFlashbackArchive();
    optionsWidget->setFlashbackEnabled(flashbackSupported,
                                       flashbackSupported ? "" : tr("Not supported by current database version"));
    mainLayout->addWidget(optionsWidget);

    QDialogButtonBox *buttonBox = DialogHelper::createButtonBox(this, QDialogButtonBox::Ok);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

TableDiffDdlOptions TableCreateLikeOptionsDialog::getOptions() const
{
    return optionsWidget->getOptions();
}
