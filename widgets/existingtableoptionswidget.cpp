#include "existingtableoptionswidget.h"
#include "util/widgethelper.h"
#include <QtGui>

ExistingTableOptionsWidget::ExistingTableOptionsWidget(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *tableOptionsLayout=new QGridLayout();

    etComments=WidgetHelper::createCheckBox(tableOptionsLayout, 0, 0, tr("Comments"), true);
    etProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 1, 0, tr("Properties"), false);
    etFlashbackArchive=WidgetHelper::createCheckBox(tableOptionsLayout, 2, 0, tr("Flashback archive"), false);
    etFlashbackArchive->setEnabled(false);
    etFlashbackArchive->setToolTip(tr("Will be enabled if both databases are version 11g or higher"));
    etStorage=WidgetHelper::createCheckBox(tableOptionsLayout, 3, 0, tr("Storage"), false);
    etIOTProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 4, 0, tr("IOT Properties"), false);
    etColumns=WidgetHelper::createCheckBox(tableOptionsLayout, 5, 0, tr("Columns"), true);
    etLOBProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 6, 0, tr("LOB Properties"), false);
    etConstraints=WidgetHelper::createCheckBox(tableOptionsLayout, 0, 1, tr("Constraints"), true);
    etIndexes=WidgetHelper::createCheckBox(tableOptionsLayout, 1, 1, tr("Indexes"), true);
    etPartitions=WidgetHelper::createCheckBox(tableOptionsLayout, 2, 1, tr("Partitions"), true);
    etExternalProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 3, 1, tr("External properties"), true);
    etTriggers=WidgetHelper::createCheckBox(tableOptionsLayout, 4, 1, tr("Triggers"), true);
    etGrants=WidgetHelper::createCheckBox(tableOptionsLayout, 5, 1, tr("Grants"), false);

    tableOptionsLayout->setContentsMargins(0,0,0,0);
    setLayout(tableOptionsLayout);

    connect(etColumns, SIGNAL(stateChanged(int)), this, SLOT(etOptionsChanged(int)));
}

TableDiffDdlOptions ExistingTableOptionsWidget::getOptions() const
{
    TableDiffDdlOptions options;

    options.comments=etComments->isChecked();
    options.properties=etProperties->isChecked();
    options.flashbackArchive=etFlashbackArchive->isChecked();
    options.storage=etStorage->isChecked();
    options.iotProperties=etIOTProperties->isChecked();
    options.lobProperties=etLOBProperties->isChecked();
    options.columns=etColumns->isChecked();
    options.constraints=etConstraints->isChecked();
    options.indexes=etIndexes->isChecked();
    options.partitions=etPartitions->isChecked();
    options.externalProperties=etExternalProperties->isChecked();
    options.triggers=etTriggers->isChecked();
    options.grants=etGrants->isChecked();

    return options;
}

void ExistingTableOptionsWidget::optionsChanged(int)
{
    etLOBProperties->setEnabled(etColumns->isChecked());
}

void ExistingTableOptionsWidget::setFlashbackEnabled(bool enabled, const QString &tooltip)
{
    etFlashbackArchive->setEnabled(enabled);
    etFlashbackArchive->setToolTip(tooltip);

    //etFlashbackArchive->setChecked(enabled);
}
