#include "storageddlexportoptionswidget.h"
#include "util/widgethelper.h"
#include <QtGui>

StorageDdlExportOptionsWidget::StorageDdlExportOptionsWidget(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *newObjectOptionsLayout=new QGridLayout();

    noStorage=WidgetHelper::createCheckBox(newObjectOptionsLayout, 0, 0, tr("Generate storage DDL"), false);
    noTablespaceOnly=WidgetHelper::createCheckBox(newObjectOptionsLayout, 0, 1, tr("Tablespace only"), true);

    noTablespaceName=new QLineEdit();
    newObjectOptionsLayout->addWidget(new QLabel(tr("Tablespace for tables")), 2, 0);
    newObjectOptionsLayout->addWidget(noTablespaceName, 2, 1);

    noIOTOverflowTablespaceName=new QLineEdit();
    newObjectOptionsLayout->addWidget(new QLabel(tr("IOT overflow tablespace")),3, 0);
    newObjectOptionsLayout->addWidget(noIOTOverflowTablespaceName, 3, 1);

    noIndexTablespaceName=new QLineEdit();
    newObjectOptionsLayout->addWidget(new QLabel(tr("Index tablespace")), 4, 0);
    newObjectOptionsLayout->addWidget(noIndexTablespaceName, 4, 1);

    noLobTablespaceName=new QLineEdit();
    newObjectOptionsLayout->addWidget(new QLabel(tr("LOB tablespace")), 5, 0);
    newObjectOptionsLayout->addWidget(noLobTablespaceName, 5, 1);

    noPartitionsTablespaceName=new QLineEdit();
    newObjectOptionsLayout->addWidget(new QLabel(tr("Partition tablespace")), 6, 0);
    newObjectOptionsLayout->addWidget(noPartitionsTablespaceName, 6, 1);

    newObjectOptionsLayout->setContentsMargins(0,0,0,0);
    setLayout(newObjectOptionsLayout);

    storageCheckBoxChanged();
    connect(noStorage, SIGNAL(stateChanged(int)), this, SLOT(storageCheckBoxChanged()));
}

NewDbObjectStorageOptions StorageDdlExportOptionsWidget::getOptions() const
{
    NewDbObjectStorageOptions options;

    options.storage=noStorage->isChecked();
    options.tablespaceOnly=noTablespaceOnly->isChecked();
    options.tablespaceName=noTablespaceName->text().trimmed().toUpper();
    options.iotOverflowTablespaceName=noIOTOverflowTablespaceName->text().trimmed().toUpper();
    options.indexTablespaceName=noIndexTablespaceName->text().trimmed().toUpper();
    options.lobTablespaceName=noLobTablespaceName->text().trimmed().toUpper();
    options.partitionsTablespaceName=noPartitionsTablespaceName->text().trimmed().toUpper();

    return options;
}

void StorageDdlExportOptionsWidget::storageCheckBoxChanged()
{
    bool storageChecked=noStorage->isChecked();
    noTablespaceOnly->setEnabled(storageChecked);
    noTablespaceName->setEnabled(storageChecked);
    noIOTOverflowTablespaceName->setEnabled(storageChecked);
    noIndexTablespaceName->setEnabled(storageChecked);
    noLobTablespaceName->setEnabled(storageChecked);
    noPartitionsTablespaceName->setEnabled(storageChecked);
}
