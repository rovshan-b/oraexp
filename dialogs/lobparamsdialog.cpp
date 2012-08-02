#include "lobparamsdialog.h"
#include "widgets/lobparamswidget.h"
#include <QtGui>

LobParamsDialog::LobParamsDialog(IQueryScheduler *queryScheduler, QWidget *parent, bool isEditMode) :
    QDialog(parent)
{
     QVBoxLayout *mainLayout=new QVBoxLayout(this);

     lobParamsWidget=new LobParamsWidget(queryScheduler, isEditMode);
     lobParamsWidget->layout()->setContentsMargins(0,0,0,0);
     mainLayout->addWidget(lobParamsWidget);

     QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
     buttonBox->setOrientation(Qt::Horizontal);
     buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
     mainLayout->addWidget(buttonBox);

     setLayout(mainLayout);

     setWindowTitle(tr("LOB parameters"));

     connect(buttonBox, SIGNAL(accepted()), this, SLOT(okButtonPressed()));
     connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
 }

LobParams LobParamsDialog::getParams() const
{
    return lobParamsWidget->getLobParams();
}

void LobParamsDialog::setParams(const LobParams &params, const LobParams &/*originalParams*/) const
{
    lobParamsWidget->setLobParams(params);
}


 void LobParamsDialog::okButtonPressed()
 {
    LobParams params=getParams();
    QString errorMessages;

    if(params.enableRetention && params.pctVersion!=-1){
        errorMessages.append(tr("You cannot specify both % Version and Retention"));
    }

    if(params.freePools!=-1 && params.storageParams.freeListGroups!=-1){
        if(!errorMessages.isEmpty()){
            errorMessages.append("\n");
        }
        errorMessages.append(tr("You cannot specify both Free Pools and the Free List Groups"));
    }

    if(!errorMessages.isEmpty()){
        QMessageBox::critical(this, tr("Incompatible values entered"), errorMessages);
        return;
    }

    accept();
 }
