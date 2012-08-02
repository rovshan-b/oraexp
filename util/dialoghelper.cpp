#include "dialoghelper.h"
#include "dialogs/storageparamsdialog.h"
#include "connectionspane.h"
#include "connectdialog.h"
#include <QtGui>

DialogHelper::DialogHelper()
{
}


QDialogButtonBox *DialogHelper::createButtonBox(QDialog *dialog, QDialogButtonBox::StandardButtons buttons)
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(dialog);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(buttons);

    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));

    if((buttons & QDialogButtonBox::Cancel)==QDialogButtonBox::Cancel){
        QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));
    }

    return buttonBox;
}

bool DialogHelper::showStorageParamsDialog(QWidget *parent, IQueryScheduler *scheduler, bool editMode, bool showPctUsedEditor, StorageParams &storageParams)
{
    StorageParamsDialog dialog(scheduler, parent, editMode, showPctUsedEditor);
    dialog.setParams(storageParams, storageParams);

    if(dialog.exec()){
        storageParams=dialog.getParams();

        return true;
    }

    return false;
}

void DialogHelper::showConnectDialog(ConnectionsPane *connectionsPane)
{
    ConnectDialog connectDialog(connectionsPane->window());
    if(connectDialog.exec())
    {
        connectionsPane->addConnection(connectDialog.getConnection());
    }
    connectDialog.saveConnectionsToConfig();
}

DbConnection *DialogHelper::getConnection(QWidget *dialogParent)
{
    ConnectDialog connectDialog(dialogParent);
    if(connectDialog.exec())
    {
        return connectDialog.getConnection();
    }

    return 0;
}
