#ifndef STORAGEDDLEXPORTOPTIONSWIDGET_H
#define STORAGEDDLEXPORTOPTIONSWIDGET_H

#include <QWidget>
#include "beans/newdbobjectstorageoptions.h"

class QCheckBox;
class QLineEdit;

class StorageDdlExportOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StorageDdlExportOptionsWidget(QWidget *parent = 0);

    NewDbObjectStorageOptions getOptions() const;

private slots:
    void storageCheckBoxChanged();
    
private:
    QCheckBox *noStorage;
    QCheckBox *noTablespaceOnly;
    QLineEdit *noTablespaceName;
    QLineEdit *noIOTOverflowTablespaceName;
    QLineEdit *noIndexTablespaceName;
    QLineEdit *noLobTablespaceName;
    QLineEdit *noPartitionsTablespaceName;
    
};

#endif // STORAGEDDLEXPORTOPTIONSWIDGET_H
