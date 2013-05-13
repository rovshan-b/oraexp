#ifndef DIALOGHELPER_H
#define DIALOGHELPER_H

class QDialog;
class QWidget;
class IQueryScheduler;
class ConnectionsPane;
class DbConnection;

#include "beans/storageparams.h"
#include <QDialogButtonBox>

class DialogHelper
{
public:
    static QDialogButtonBox *createButtonBox(QDialog *dialog, QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    static bool showStorageParamsDialog(QWidget *parent, IQueryScheduler *scheduler, bool editMode, bool showPctUsedEditor, StorageParams &storageParams);

    static void showConnectDialog(ConnectionsPane *connectionsPane);
    static DbConnection *getConnection(QWidget *dialogParent);

    static QString showFileSaveDialog(QWidget *parent,
                                      const QString &defaultSuffix,
                                      const QString &title = QObject::tr("Save file"),
                                      const QString &filter = QObject::tr("All files (*.*)"));

    static QString showFileOpenDialog(QWidget *parent,
                                      const QString &filter = QObject::tr("All files (*.*)"));

    static void centerWindow(QWidget *window);

private:
    DialogHelper();
};

#endif // DIALOGHELPER_H
