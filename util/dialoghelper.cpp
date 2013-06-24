#include "dialoghelper.h"
#include "dialogs/storageparamsdialog.h"
#include "connectionspane.h"
#include "connectdialog.h"
#include "util/settings.h"
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

QString DialogHelper::showFileSaveDialog(QWidget *parent,
                                         const QString &defaultFileName,
                                         const QString &defaultSuffix,
                                         const QString &title, const QString &filter)
{
    QString directory = Settings::value("saveFileDialogDirectory",
                                        QDesktopServices::storageLocation(QDesktopServices::DesktopLocation)).toString();

    if(!defaultFileName.isEmpty()){
        QFileInfo fileInfo(defaultFileName);
        if(fileInfo.isAbsolute()){
            directory = defaultFileName;
        }else{
            if(!directory.endsWith('/') && !directory.endsWith('\\')){
                directory.append('/');
            }
            directory.append(defaultFileName);
        }
    }

    QString result = QFileDialog::getSaveFileName(parent->window(),
                                        title,
                                        directory,
                                        filter);
    if(!result.isEmpty()){
        QFileInfo info(result);
        if(info.suffix().isEmpty()){
            result.append(".").append(defaultSuffix);
        }

        Settings::setValue("saveFileDialogDirectory", info.path());
    }

    return result;
    /*
    QFileDialog dialog(parent, title, QDesktopServices::storageLocation(QDesktopServices::DesktopLocation), filter);
    dialog.setDefaultSuffix(defaultSuffix);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setConfirmOverwrite(true);

    QByteArray state = Settings::value("saveFileDialogState").toByteArray();
    if(!state.isEmpty()){
        dialog.restoreState(state);
    }

    QString result;

    if(dialog.exec()){
        result = dialog.selectedFiles().at(0);

        Settings::setValue("saveFileDialogState", dialog.saveState());
    }

    return result;*/
}

QString DialogHelper::showFileOpenDialog(QWidget *parent, const QString &filter)
{
    QString directory = Settings::value("saveFileDialogDirectory",
                                        QDesktopServices::storageLocation(QDesktopServices::DesktopLocation)).toString();
    QString result = QFileDialog::getOpenFileName(parent,
                                                  QObject::tr("Open file"),
                                                  directory,
                                                  filter);
    if(!result.isEmpty()){
        Settings::setValue("saveFileDialogDirectory", QFileInfo(result).path());
    }

    return result;
}

QStringList DialogHelper::showFilesOpenDialog(QWidget *parent, const QString &filter)
{
    QString directory = Settings::value("saveFileDialogDirectory",
                                             QDesktopServices::storageLocation(QDesktopServices::DesktopLocation)).toString();
    QStringList result = QFileDialog::getOpenFileNames(parent,
                                                  QObject::tr("Open file"),
                                                  directory,
                                                  filter);
    if(!result.isEmpty()){
        Settings::setValue("saveFileDialogDirectory", QFileInfo(result.at(0)).path());
    }

    return result;
}

void DialogHelper::centerWindow(QWidget *window)
{
    QWidgetList topLevelWidgets = qApp->topLevelWidgets();
    if(topLevelWidgets.isEmpty()){
        return;
    }

    QRect frect = window->frameGeometry();
    QDesktopWidget *desktop=qApp->desktop();
    frect.moveCenter(desktop->availableGeometry(topLevelWidgets.at(0)).center());
    window->move(frect.topLeft());
}
