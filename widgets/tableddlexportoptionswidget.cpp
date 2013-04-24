#include "tableddlexportoptionswidget.h"
#include "util/widgethelper.h"
#include <QtGui>

TableDdlExportOptionsWidget::TableDdlExportOptionsWidget(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *tableOptionsLayout=new QGridLayout();

    ntProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 0, 0, tr("Properties"), false);
    ntFlashbackArchive=WidgetHelper::createCheckBox(tableOptionsLayout, 1, 0, tr("Flashback archive"), false);
    ntFlashbackArchive->setEnabled(false);
    ntFlashbackArchive->setToolTip(tr("Will be enabled if both databases are version 11g or higher"));
    ntIOTProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 2, 0, tr("IOT properties"), true);
    ntLobProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 3, 0, tr("LOB properties"), false);
    ntIndexes=WidgetHelper::createCheckBox(tableOptionsLayout, 0, 1, tr("Indexes"), true);

    ntTriggers=WidgetHelper::createCheckBox(tableOptionsLayout, 1, 1, tr("Triggers"), true);
    ntGrants=WidgetHelper::createCheckBox(tableOptionsLayout, 2, 1, tr("Grants"), false);

    tableOptionsLayout->setContentsMargins(0,0,0,0);
    setLayout(tableOptionsLayout);
}

TableCreateDdlOptions TableDdlExportOptionsWidget::getOptions() const
{
    TableCreateDdlOptions options;

    options.properties=ntProperties->isChecked();
    options.flashbackArchive=ntFlashbackArchive->isChecked();
    options.iotProperties=ntIOTProperties->isChecked();
    options.lobProperties=ntLobProperties->isChecked();
    options.indexes=ntIndexes->isChecked();
    options.triggers=ntTriggers->isChecked();
    options.grants=ntGrants->isChecked();

    return options;
}

void TableDdlExportOptionsWidget::setFlashbackEnabled(bool enabled, const QString &tooltip)
{
    ntFlashbackArchive->setEnabled(enabled);
    ntFlashbackArchive->setToolTip(tooltip);

    ntFlashbackArchive->setChecked(enabled);
}
