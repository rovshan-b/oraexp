#include "tablecreatorexternalpropertiesgeneralinfowidget.h"
#include "widgets/datatable.h"
#include "widgets/extentsizeeditorwidget.h"
#include "widgets/datatableandtoolbarwidget.h"
#include "widgets/comboboxandlineeditwidget.h"
#include "connectivity/dbconnection.h"
#include "connectivity/resultset.h"
#include "connectivity/ociexception.h"
#include "util/widgethelper.h"
#include "util/iconutil.h"
#include "util/strutil.h"
#include "models/genericeditabletablemodel.h"
#include "delegates/comboboxdelegate.h"
#include "delegates/autoappenddelegate.h"
#include "connection_page/table_creator/tablecreatortabs.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

TableCreatorExternalPropertiesGeneralInfoWidget::TableCreatorExternalPropertiesGeneralInfoWidget(IQueryScheduler *queryScheduler,
                                                                                                 DbObjectCreator::CreatorMode creatorMode,
                                                                                                 TableCreatorTabs *tableCreator,
                                                                                                 QWidget *parent) :
    QWidget(parent), originalExternalInfo(0), tableCreator(tableCreator)
{
    QFormLayout *layout=new QFormLayout();

    externalTableTypeComboBox=new QComboBox();
    QIcon engineIcon=IconUtil::getIcon("engine");
    externalTableTypeComboBox->addItem(engineIcon, "ORACLE_LOADER");
    externalTableTypeComboBox->addItem(engineIcon, "ORACLE_DATAPUMP");
    externalTableTypeComboBox->setCurrentIndex(0);
    if(creatorMode == DbObjectCreator::EditExisting){
        externalTableTypeComboBox->setEnabled(false);
    }
    layout->addRow(tr("Type"), externalTableTypeComboBox);

    defaultDirectoryComboBox=new QComboBox();
    defaultDirectoryComboBox->setEditable(true);
    layout->addRow(tr("Default directory"), defaultDirectoryComboBox);

    setupLocationsTable();
    layout->addRow(tr("Locations"), locationsTable);

    rejectLimitEditor=new ExtentSizeEditorWidget(false, true);
    layout->addRow(tr("Reject limit"), rejectLimitEditor);

    projectColumnComboBox=new QComboBox();
    projectColumnComboBox->addItem("");
    projectColumnComboBox->addItem(tr("All"));
    projectColumnComboBox->addItem(tr("Referenced"));
    projectColumnComboBox->setCurrentIndex(0);
    if(!queryScheduler->getDb()->supportsExternalTableProjectColumn()){
        projectColumnComboBox->setEnabled(false);
        projectColumnComboBox->setToolTip(tr("Oracle 10g or later"));
    }
    layout->addRow(tr("Project column"), projectColumnComboBox);

    usingClobCheckBox=new QCheckBox();
    if(creatorMode == DbObjectCreator::EditExisting){
        usingClobCheckBox->setEnabled(false);
    }
    layout->addRow(tr("Using CLOB subquery"), usingClobCheckBox);

    parallelEditor=new ComboBoxAndLineEditWidget(true, tr("[Parallel degree]"));
    parallelEditor->comboBox()->addItem("");
    parallelEditor->comboBox()->addItem(tr("Enable"));
    parallelEditor->comboBox()->addItem(tr("Disable"));
    parallelEditor->setToolTip(tr("Enabling this option will disable specifying number of rows to SKIP with ORACLE_LOADER driver"));
    layout->addRow(tr("Parallel"), parallelEditor);

    setLayout(layout);

    connect(externalTableTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(driverChanged(int)));
    connect(externalTableTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(defaultDirectoryComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(locationsTable->table()->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
    connect(rejectLimitEditor, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
    connect(projectColumnComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(usingClobCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(ddlChanged()));
    connect(parallelEditor, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
    connect(parallelEditor, SIGNAL(changed()), this, SLOT(parallelEditorChanged()));
}

void TableCreatorExternalPropertiesGeneralInfoWidget::directoryListAvailable(const QStringList &directoryList)
{
    WidgetHelper::populateComboBox(defaultDirectoryComboBox, directoryList, "directory");

    locationsTableModel->setList(0, directoryList);
}

void TableCreatorExternalPropertiesGeneralInfoWidget::parallelEditorChanged()
{
    emit parallelChanged(parallelEditor);
}

void TableCreatorExternalPropertiesGeneralInfoWidget::setupLocationsTable()
{
    QStringList locationTableHeaders;
    locationTableHeaders.append(tr("Directory"));
    locationTableHeaders.append(tr("File name"));
    locationsTableModel=new GenericEditableTableModel(locationTableHeaders, this);
    locationsTableModel->setTitleColumn(1);
    locationsTable=new DataTableAndToolBarWidget(locationsTableModel);

    locationsTable->table()->horizontalHeader()->setDefaultSectionSize(200);
    locationsTable->table()->verticalHeader()->setVisible(false);
    locationsTable->table()->setEditTriggers(QAbstractItemView::AllEditTriggers);

    QPixmap directoryIcon=IconUtil::getIcon("directory");
    ComboBoxDelegate *directoryDelegate=new ComboBoxDelegate(this, 0, true);
    locationsTableModel->setColumnIcon(0, directoryIcon);
    locationsTable->table()->setItemDelegateForColumn(0, directoryDelegate);
    AutoAppendDelegate *filenameDelegate=new AutoAppendDelegate(this);
    locationsTable->table()->setItemDelegateForColumn(1, filenameDelegate);
}

int TableCreatorExternalPropertiesGeneralInfoWidget::selectedDriverIndex() const
{
    return externalTableTypeComboBox->currentIndex();
}

TableExternalInfo TableCreatorExternalPropertiesGeneralInfoWidget::itemInfoFromWidgets() const
{
    TableExternalInfo info;

    info.driverType=externalTableTypeComboBox->currentText();
    info.defaultDirectory=defaultDirectoryComboBox->currentText();
    info.rejectLimitUnlimited=rejectLimitEditor->isUnlimited();
    info.rejectLimit=getEditorIntValue(rejectLimitEditor->lineEdit()->text().trimmed());
    info.projectColumn=projectColumnComboBox->currentIndex();
    info.usingClobSubquery=usingClobCheckBox->isChecked();
    info.parallel=parallelEditor->comboBox()->currentIndex();
    info.parallelDegree=getEditorIntValue(parallelEditor->lineEdit()->text().trimmed());

    QAbstractItemModel *model=locationsTable->table()->model();
    int itemCount=model->rowCount();
    QString fileName, directoryName;
    NameQueryPair item;
    for(int i=0; i<itemCount; ++i){
        fileName=model->index(i, 1).data().toString().trimmed();
        if(!fileName.isEmpty()){
            item.second=fileName;

            directoryName=model->index(i, 0).data().toString().trimmed();
            if(!directoryName.isEmpty()){
                item.first=directoryName;
            }

            info.locations.append(item);
        }
    }

    return info;
}

void TableCreatorExternalPropertiesGeneralInfoWidget::setAttributes(TableExternalInfo *info)
{
    originalExternalInfo=info;

    WidgetHelper::setComboBoxText(externalTableTypeComboBox, info->driverType);
    WidgetHelper::setComboBoxText(defaultDirectoryComboBox, info->defaultDirectory);

    if(info->rejectLimitUnlimited){
       rejectLimitEditor->setUnlimited(true);
    }else if(info->rejectLimit!=-1){
       rejectLimitEditor->setText(QString::number(info->rejectLimit));
    }

    projectColumnComboBox->setCurrentIndex(info->projectColumn);

    parallelEditor->comboBox()->setCurrentIndex(info->parallel);
    if(info->parallelDegree!=-1){
        parallelEditor->lineEdit()->setText(QString::number(info->parallelDegree));
    }

    populateLocationsTable();
}

void TableCreatorExternalPropertiesGeneralInfoWidget::populateLocationsTable()
{
    int locationCount=originalExternalInfo->locations.size();
    GenericEditableTableModel *model=static_cast<GenericEditableTableModel*>(locationsTable->table()->model());
    Q_ASSERT(model);

    model->ensureRowCount(locationCount);

    for(int i=0; i<locationCount; ++i){
        model->setData(model->index(i, 0), originalExternalInfo->locations.at(i).first);
        model->setData(model->index(i, 1), originalExternalInfo->locations.at(i).second);
    }
}
