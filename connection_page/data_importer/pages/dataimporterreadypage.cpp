#include "dataimporterreadypage.h"
#include "util/iconutil.h"
#include <QtGui>

DataImporterReadyPage::DataImporterReadyPage(QWidget *parent) :
    ConnectionPageWizardPage(parent)
{
    setTitle(tr("Ready to start"));
    setSubTitle(tr("Review presented information and press Next button to start importing data"));
    //setCommitPage(true);

    QFormLayout *mainLayout = new QFormLayout();

    schemaNameLabel = new QLabel();
    schemaNameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    mainLayout->addRow(tr("Schema"), schemaNameLabel);

    tableNameLabel = new QLabel();
    tableNameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    mainLayout->addRow(tr("Table"), tableNameLabel);

    fileNameLabel = new QLabel();
    fileNameLabel->setWordWrap(true);
    fileNameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    mainLayout->addRow(tr("Filename"), fileNameLabel);

    beforeImportQueryLabel = new QLabel();
    beforeImportQueryLabel->setWordWrap(true);
    beforeImportQueryLabel->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    mainLayout->addRow(tr("Before import query"), beforeImportQueryLabel);

    importQueryLabel = new QLabel();
    importQueryLabel->setWordWrap(true);
    importQueryLabel->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    mainLayout->addRow(tr("Import query"), importQueryLabel);

    afterImportQueryLabel = new QLabel();
    afterImportQueryLabel->setWordWrap(true);
    afterImportQueryLabel->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    mainLayout->addRow(tr("After import query"), afterImportQueryLabel);

    setLayout(mainLayout);
}

void DataImporterReadyPage::initializePage()
{
    schemaNameLabel->setText(QString("<img src=':/images/user.png' /> %1").arg(field("schemaName").toString().trimmed().toUpper()));
    tableNameLabel->setText(QString("<img src=':/images/table.png' /> %1").arg(field("tableName").toString().trimmed().toUpper()));
    fileNameLabel->setText(QString("<img src=':/images/fileopen.png' /> %1").arg(field("fileName").toString()));
    beforeImportQueryLabel->setText(field("beforeImportGroupBox").toBool() ? field("beforeImportQuery").toString().trimmed() : tr("Empty"));
    importQueryLabel->setText(field("importQuery").toString());
    afterImportQueryLabel->setText(field("afterImportGroupBox").toBool() ? field("afterImportQuery").toString().trimmed() : tr("Empty"));
}
