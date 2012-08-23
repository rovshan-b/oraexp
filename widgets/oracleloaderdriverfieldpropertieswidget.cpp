#include "oracleloaderdriverfieldpropertieswidget.h"
#include "widgets/comboboxandlineeditwidget.h"
#include "widgets/datatable.h"
#include "widgets/lineeditandcheckboxwidget.h"
#include "models/genericeditabletablemodel.h"
#include "widgets/datatableandtoolbarwidget.h"
#include "widgets/tabletoolbar.h"
#include "delegates/identifiernamedelegate.h"
#include "util/iconutil.h"
#include "models/externaltablefieldsmodel.h"
#include "models/externaltablecolumntransformsmodel.h"
#include "delegates/comboboxdelegate.h"
#include "delegates/columnselectordelegate.h"
#include "delegates/dynamiccomboboxdelegate.h"
#include "connection_page/table_creator/tablecreatortabs.h"
#include "connection_page/table_creator/tabs/tablecreatorcolumns.h"
#include <QtGui>

OracleLoaderDriverFieldPropertiesWidget::OracleLoaderDriverFieldPropertiesWidget(IQueryScheduler *queryScheduler, IStringListRetriever *columnListRetriever, QWidget *parent) :
    QWidget(parent), fieldNameRetriever(this)
{
    QVBoxLayout *layout=new QVBoxLayout();

    QFormLayout *form=new QFormLayout();

    QStringList itemsToDisable;
    itemsToDisable.append(tr("Whitespace"));
    terminatorSelector=new ComboBoxAndLineEditWidget(false, tr("Field terminator"), itemsToDisable);
    terminatorSelector->comboBox()->addItem("");
    terminatorSelector->comboBox()->addItem(tr("Whitespace"));
    terminatorSelector->comboBox()->addItem(tr("String"));
    terminatorSelector->comboBox()->setCurrentIndex(0);
    form->addRow(tr("Fields terminated by"), terminatorSelector);

    enclosedByEditor=new LineEditAndCheckBoxWidget(tr("Optionally"));
    enclosedByEditor->lineEdit()->setToolTip(tr("Comma separated list of characters"));
    form->addRow(tr("Enclosed by"), enclosedByEditor);

    trimTypeComboBox=new QComboBox();
    QStringList trimTypes;
    fillTrimTypes(trimTypes);
    trimTypeComboBox->addItems(trimTypes);
    form->addRow(tr("Trim"), trimTypeComboBox);

    missingFieldValuesNullCheckBox=new QCheckBox();
    form->addRow(tr("Missing field values are null"), missingFieldValuesNullCheckBox);

    rejectNullRowsCheckBox=new QCheckBox();
    form->addRow(tr("Reject null rows"), rejectNullRowsCheckBox);

    layout->addLayout(form);

    fieldsTab=new QTabWidget();
    fieldsTab->setTabPosition(QTabWidget::South);
    createFieldsTable(queryScheduler);
    createColumnTransformsTable(columnListRetriever);
    fieldsTab->addTab(fieldList, tr("Fields"));
    fieldsTab->addTab(columnTransformsTable, tr("Column transforms"));

    layout->addWidget(fieldsTab);

    setLayout(layout);

    connect(enclosedByEditor->checkBox(), SIGNAL(toggled(bool)), this, SLOT(enclosedIsOptionalToggled(bool)));

    connect(terminatorSelector, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
    connect(enclosedByEditor, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
    connect(trimTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(missingFieldValuesNullCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(ddlChanged()));
    connect(rejectNullRowsCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(ddlChanged()));
    connect(fieldList->table()->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
    connect(columnTransformsTable->table()->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void OracleLoaderDriverFieldPropertiesWidget::enclosedIsOptionalToggled(bool checked)
{
    if(checked && terminatorSelector->comboBox()->currentIndex()==0){
        QMessageBox::information(NULL, tr("Field terminator not specified"),
                                 tr("When selecting \"Enclosed by\" as optional, value of \"Fields terminated by\" field "
                                    "must also be entered."));
    }
}

void OracleLoaderDriverFieldPropertiesWidget::createFieldsTable(IQueryScheduler *)
{
    QStringList colNames;
    colNames.append(tr("Name"));
    colNames.append(tr("Position spec"));
    colNames.append(tr("Data type"));
    colNames.append(tr("Terminated by"));
    colNames.append(tr("Enclosed by"));
    colNames.append(tr("Trim"));
    colNames.append(tr("Date type"));
    colNames.append(tr("Date mask"));
    colNames.append(tr("NULLIF"));
    colNames.append(tr("DEFAULTIF"));
    ExternalTableFieldsModel *fieldListModel=new ExternalTableFieldsModel(colNames, this);
    fieldList=new DataTableAndToolBarWidget(fieldListModel);
    DataTable *table=fieldList->table();
    table->setToolTip(tr("All columns are optional"));
    table->resizeColumnsToContents();
    table->setColumnWidth(ExternalTableFieldsModel::FieldName, 150);
    table->setColumnWidth(ExternalTableFieldsModel::FieldDataType, 150);
    table->setColumnWidth(ExternalTableFieldsModel::FieldTrim, 100);
    table->setColumnWidth(ExternalTableFieldsModel::FieldDateType, 150);
    table->setColumnWidth(ExternalTableFieldsModel::FieldDateMask, 100);
    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    IdentifierNameDelegate *nameDelegate=new IdentifierNameDelegate(this);
    table->setItemDelegateForColumn(ExternalTableFieldsModel::FieldName, nameDelegate);

    QStringList dataTypeList;
    dataTypeList.append("");
    dataTypeList.append("CHAR(255)");
    dataTypeList.append("INTEGER");
    dataTypeList.append("INTEGER EXTERNAL");
    dataTypeList.append("UNSIGNED INTEGER");
    dataTypeList.append("UNSIGNED INTEGER EXTERNAL");
    dataTypeList.append("DECIMAL");
    dataTypeList.append("DECIMAL EXTERNAL");
    dataTypeList.append("ZONED");
    dataTypeList.append("ZONED EXTERNAL");
    dataTypeList.append("ORACLE_DATE");
    dataTypeList.append("ORACLE_NUMBER");
    dataTypeList.append("ORACLE_NUMBER COUNTED");
    dataTypeList.append("FLOAT");
    dataTypeList.append("FLOAT EXTERNAL");
    dataTypeList.append("DOUBLE");
    dataTypeList.append("BINARY FLOAT");
    dataTypeList.append("BINARY FLOAT EXTERNAL");
    dataTypeList.append("BINARY_DOUBLE");
    dataTypeList.append("RAW");
    dataTypeList.append("VARCHAR(255)");
    dataTypeList.append("VARRAW(255)");
    dataTypeList.append("VARCHARC(255)");
    dataTypeList.append("VARRAWC(255)");
    ComboBoxDelegate *dataTypeDelegate=new ComboBoxDelegate(this, true, QIcon(), dataTypeList);
    table->setItemDelegateForColumn(ExternalTableFieldsModel::FieldDataType, dataTypeDelegate);

    QStringList trimTypes;
    fillTrimTypes(trimTypes);
    ComboBoxDelegate *trimTypesDelegate=new ComboBoxDelegate(this, false, QIcon(), trimTypes);
    table->setItemDelegateForColumn(ExternalTableFieldsModel::FieldTrim, trimTypesDelegate);

    QStringList dateTypes;
    dateTypes.append("");
    dateTypes.append("DATE");
    dateTypes.append("TIMESTAMP WITH TIME ZONE");
    dateTypes.append("TIMESTAMP WITH LOCAL TIME ZONE");
    dateTypes.append("INTERVAL YEAR_TO_MONTH");
    dateTypes.append("INTERVAL DAY_TO_SECOND");
    ComboBoxDelegate *dateTypeDelegate=new ComboBoxDelegate(this, false, QIcon(), dateTypes);
    table->setItemDelegateForColumn(ExternalTableFieldsModel::FieldDateType, dateTypeDelegate);

    fieldList->toolBar()->addSeparator();
    fieldList->toolBar()->addAction(IconUtil::getIcon("column"), tr("Copy from columns tab"), this, SLOT(populateFieldListFromColumnsTab()));
}

void OracleLoaderDriverFieldPropertiesWidget::populateFieldListFromColumnsTab()
{

}

void OracleLoaderDriverFieldPropertiesWidget::fillTrimTypes(QStringList &list)
{
    list.append("");
    list.append(tr("LRTRIM"));
    list.append(tr("LTRIM"));
    list.append(tr("RTRIM"));
    list.append(tr("LDRTRIM"));
    list.append(tr("NOTRIM"));
}

void OracleLoaderDriverFieldPropertiesWidget::createColumnTransformsTable(IStringListRetriever *columnListRetriever)
{
    QStringList colNames;
    colNames.append(tr("Column"));
    colNames.append(tr("Transform type"));
    colNames.append(tr("String/Field names"));
    colNames.append(tr("LOB type"));
    colNames.append(tr("Directories"));
    colNames.append(tr("Character set"));
    ExternalTableColumnTransformsModel *columnTransformsModel=new ExternalTableColumnTransformsModel(colNames, this);
    columnTransformsTable=new DataTableAndToolBarWidget(columnTransformsModel);
    DataTable *table=columnTransformsTable->table();

    table->resizeColumnsToContents();
    table->setColumnWidth(ExternalTableColumnTransformsModel::TransformColumn, 150);
    table->setColumnWidth(ExternalTableColumnTransformsModel::TransformType, 150);
    table->setColumnWidth(ExternalTableColumnTransformsModel::TransformDirectories, 150);

    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    ColumnSelectorDelegate *columnsDelegate=new ColumnSelectorDelegate(columnListRetriever, tr("Select columns"), this);
    table->setItemDelegateForColumn(ExternalTableColumnTransformsModel::TransformColumn, columnsDelegate);

    QStringList fromList;
    fromList.append("");
    fromList.append(tr("NULL"));
    fromList.append(tr("CONSTANT"));
    fromList.append(tr("CONCAT"));
    fromList.append(tr("LOBFILE"));
    ComboBoxDelegate *fromDelegate=new ComboBoxDelegate(this, false, QIcon(), fromList);
    table->setItemDelegateForColumn(ExternalTableColumnTransformsModel::TransformType, fromDelegate);

    ColumnSelectorDelegate *fieldNameDelegate=new ColumnSelectorDelegate(&fieldNameRetriever, tr("Select fields"), this, false);
    fieldNameDelegate->setAutoConvertToUpperCase(false);
    fieldNameDelegate->setTooltip(tr("Examples:\n\tWhen Tranform type is CONSTANT: some_string"
                                     "\n\tWhen Transform type is CONCAT: field_name, CONSTANT 'some_string', ..."
                                     "\n\tWhen Transform type is LOBFILE: field_name, CONSTANT 'some_string':, ..."));
    table->setItemDelegateForColumn(ExternalTableColumnTransformsModel::TransformFieldName, fieldNameDelegate);

    QStringList lobTypeList;
    lobTypeList.append("");
    lobTypeList.append(tr("CLOB"));
    lobTypeList.append(tr("BLOB"));
    ComboBoxDelegate *lobTypeDelegate=new ComboBoxDelegate(this, false, QIcon(), lobTypeList);
    table->setItemDelegateForColumn(ExternalTableColumnTransformsModel::TransformLobType, lobTypeDelegate);

    ColumnSelectorDelegate *foldersDelegate=new ColumnSelectorDelegate(this, tr("Select directories"), this);
    table->setItemDelegateForColumn(ExternalTableColumnTransformsModel::TransformDirectories, foldersDelegate);
}

QStringList OracleLoaderDriverFieldPropertiesWidget::FieldNameRetriever::getStringList(const QString &)
{
    QAbstractItemModel *model=outer->fieldList->table()->model();
    int rowCount=model->rowCount();

    QStringList items;
    QString itemText;
    for(int i=0; i<rowCount; ++i){
        itemText=model->index(i, ExternalTableFieldsModel::FieldName).data().toString().trimmed();
        if(!itemText.isEmpty()){
            items.append(itemText);
        }
    }

    return items;
}

QString OracleLoaderDriverFieldPropertiesWidget::generateDdl() const
{
    QString ddl;

    int terminatorType=terminatorSelector->comboBox()->currentIndex();
    if(terminatorType==1){
        ddl.append("\n\tTERMINATED BY WHITESPACE");
    }else if(terminatorType==2){
        ddl.append("\n\tTERMINATED BY '").append(terminatorSelector->lineEdit()->text()).append("'");
    }

    QString enclosedBy=enclosedByEditor->lineEdit()->text();
    if(!enclosedBy.isEmpty()){
        if(enclosedByEditor->checkBox()->isChecked()){
            ddl.append("\n\tOPTIONALLY");
        }
        ddl.append("\n\tENCLOSED BY ");
        QStringList splitted=enclosedBy.split(',');
        for(int i=0; i<splitted.size(); ++i){
            if(i>0){
                ddl.append(" AND ");
            }

            ddl.append("'").append(splitted.at(i)).append("'");
        }
    }

    QString trim=trimTypeComboBox->currentText();
    if(!trim.isEmpty()){
        ddl.append("\n\t").append(trim);
    }

    if(missingFieldValuesNullCheckBox->isChecked()){
        ddl.append("\n\tMISSING FIELD VALUES ARE NULL");
    }

    if(rejectNullRowsCheckBox->isChecked()){
        ddl.append("\n\tREJECT ROWS WITH ALL NULL FIELDS");
    }

    QString fieldDefinitionsDdl=generateFieldDefinitionsDdl();
    if(!fieldDefinitionsDdl.isEmpty()){
        ddl.append("\n\t(").append(fieldDefinitionsDdl).append("\n\t)");
    }

    QString columnTransformsDdl=generateColumnTransformsDdl();
    if(!columnTransformsDdl.isEmpty()){
        ddl.append("\n\tCOLUMN TRANSFORMS (").append(columnTransformsDdl).append(")");
    }

    if(ddl.length()>0){
        ddl.prepend("\n\tFIELDS");
    }

    return ddl;
}

QString OracleLoaderDriverFieldPropertiesWidget::generateFieldDefinitionsDdl() const
{
    QString ddl;

    QAbstractItemModel *model=fieldList->table()->model();
    int itemCount=model->rowCount();

    QString fieldName;
    QString positionSpec;
    QString dataType;
    QString terminatedBy;
    QString enclosedBy;
    QString trim;
    QString dateType;
    QString dateMask;
    QString nullIf;
    QString defaultIf;


    for(int i=0; i<itemCount; ++i){
        fieldName=model->index(i, ExternalTableFieldsModel::FieldName).data().toString().trimmed();

        if(fieldName.isEmpty()){
            continue;
        }

        positionSpec=model->index(i, ExternalTableFieldsModel::FieldPosSpec).data().toString().trimmed();
        dataType=model->index(i, ExternalTableFieldsModel::FieldDataType).data().toString().trimmed();

        if(!ddl.isEmpty()){
            ddl.append(",");
        }

        ddl.append("\n\t\t").append(fieldName);

        if(!positionSpec.isEmpty()){
            ddl.append(" POSITION (").append(positionSpec).append(")");
        }

        if(!dataType.isEmpty()){
            ddl.append(" ").append(dataType);
        }

        terminatedBy=model->index(i, ExternalTableFieldsModel::FieldTerminatedBy).data().toString();
        enclosedBy=model->index(i, ExternalTableFieldsModel::FieldEnclosedBy).data().toString();

        if(!terminatedBy.isEmpty()){
            ddl.append(" TERMINATED BY '").append(terminatedBy).append("'");
        }

        if(!enclosedBy.isEmpty()){
            if(!terminatedBy.isEmpty()){
                ddl.append(" OPTIONALLY");
            }

            ddl.append(" ENCLOSED BY ");
            QStringList splitted=enclosedBy.split(',');
            for(int i=0; i<splitted.size(); ++i){
                if(i>0){
                    ddl.append(" AND ");
                }

                ddl.append("'").append(splitted.at(i)).append("'");
            }
        }

        trim=model->index(i, ExternalTableFieldsModel::FieldTrim).data().toString();

        if(!trim.isEmpty()){
            ddl.append(" ").append(trim);
        }

        dateType=model->index(i, ExternalTableFieldsModel::FieldDateType).data().toString().trimmed();
        if(!dateType.isEmpty()){
            ddl.append(" DATE_FORMAT ").append(dateType);

            if(model->flags(model->index(i, ExternalTableFieldsModel::FieldDateMask))!=Qt::NoItemFlags){
                dateMask=model->index(i, ExternalTableFieldsModel::FieldDateMask).data().toString().trimmed();
                if(!dateMask.isEmpty()){
                    ddl.append(" MASK \"").append(dateMask).append("\"");
                }
            }
        }

        nullIf=model->index(i, ExternalTableFieldsModel::FieldNullIf).data().toString().trimmed();
        defaultIf=model->index(i, ExternalTableFieldsModel::FieldDefaultIf).data().toString().trimmed();

        if(!nullIf.isEmpty()){
            ddl.append(" NULLIF ").append(nullIf);
        }

        if(!defaultIf.isEmpty()){
            ddl.append(" DEFAULTIF ").append(defaultIf);
        }

    }

    return ddl;
}

QString OracleLoaderDriverFieldPropertiesWidget::generateColumnTransformsDdl() const
{
    QString ddl;

    QAbstractItemModel *model=columnTransformsTable->table()->model();
    int itemCount=model->rowCount();

    QString columnName;
    QString transformType;
    QString fieldName;
    QString directories;
    QString lobType;
    QString characterSet;

    for(int i=0; i<itemCount; ++i){
        columnName=model->index(i, ExternalTableColumnTransformsModel::TransformColumn).data().toString().trimmed();
        transformType=model->index(i, ExternalTableColumnTransformsModel::TransformType).data().toString();

        if(columnName.isEmpty() || transformType.isEmpty()){
            continue;
        }

        if(!ddl.isEmpty()){
            ddl.append(",\n\t\t");
        }

        ddl.append("\n\t\t").append(columnName).append(" FROM ").append(transformType);

        if(transformType==tr("NULL")){
            continue;
        }

        fieldName=model->index(i, ExternalTableColumnTransformsModel::TransformFieldName).data().toString();
        if(transformType==tr("CONSTANT")){
            ddl.append(" '").append(fieldName).append("'");
        }else{
            ddl.append(" (").append(fieldName).append(")");
        }

        if(transformType!=tr("LOBFILE")){
            continue;
        }

        directories=model->index(i, ExternalTableColumnTransformsModel::TransformDirectories).data().toString().trimmed();
        if(!directories.isEmpty()){
            ddl.append(" FROM ( ").append(directories).append(" )");
        }

        lobType=model->index(i, ExternalTableColumnTransformsModel::TransformLobType).data().toString();
        if(!lobType.isEmpty()){
            ddl.append(" ").append(lobType);
        }

        characterSet=model->index(i, ExternalTableColumnTransformsModel::TransformCharacterset).data().toString().trimmed();
        if(!characterSet.isEmpty()){
            ddl.append(" CHARACTERSET = ").append(characterSet);
        }
    }

    return ddl;
}
