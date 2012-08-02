#include "tablecreatorpartitionstab.h"
#include "widgets/lineeditwithbutton.h"
#include "widgets/lineeditwithautocompleter.h"
#include "widgets/tabletoolbar.h"
#include "connection_page/table_creator/tablecreatortabs.h"
#include "connection_page/table_creator/tabs/tablecreatorcolumns.h"
#include "connection_page/table_creator/tabs/tablecreatorconstraints.h"
#include "connection_page/table_creator/tabs/tablecreatorpartitions.h"
#include "dialogs/columnselectordialog.h"
#include "delegates/identifiernamedelegate.h"
#include "delegates/columnselectordelegate.h"
#include "delegates/comboboxdelegate.h"
#include "util/strutil.h"
#include "util/widgethelper.h"
#include "util/iconutil.h"
#include "util/itemcreatorhelper.h"
#include "connectivity/dbconnection.h"
#include "connectivity/ociexception.h"
#include <QtGui>
#include <QDebug>


TableCreatorPartitionsTab::TableCreatorPartitionsTab(TableCreatorTabs* tableCreator,
                                                     OraExp::PartitionPart configureFor,
                                                     bool configureForIndex,
                                                     QWidget *parent) :
    QWidget(parent),
    //tableCreator(tableCreator),
    parentTab(0),
    configureFor(configureFor),
    foreignKeyRetriever(tableCreator->getConstraintsTab()),
    partitioningStrategy(0),
    partitionType(0),
    columnSelector(0),
    intervalSelector(0),
    foreignKeySelector(0),
    currentPartitionType(OraExp::PartitionTypeRange),
    configureForIndex(configureForIndex),
    supportsIntervalAndReferencePartitioning(false),
    editMode(false)
{
    QVBoxLayout *layout=new QVBoxLayout();

    QList< QPair<QString,QWidget*> > widgets;

    if(configureFor==OraExp::PartitionPartPartition){
        partitioningStrategy=new QComboBox(this);
        partitioningStrategy->addItem(tr("Single-Level Partitioning"));
        partitioningStrategy->addItem(tr("Composite Partitioning"));

        if(configureForIndex){
            partitioningStrategy->hide();
        }else{
            widgets.append(qMakePair(tr("Partitioning strategy"), (QWidget*)partitioningStrategy));
            connect(partitioningStrategy, SIGNAL(currentIndexChanged(int)), this, SIGNAL(partitioningStrategyChanged(int)));
        }
    }

    if(configureFor!=OraExp::PartitionPartSubpartitionTemplate){
        partitionType=new QComboBox();
        partitionType->addItem(tr("Range"), (int)OraExp::PartitionTypeRange);
        if(!configureForIndex){
            partitionType->addItem(tr("List"), (int)OraExp::PartitionTypeList);
        }
        //other types of partitioning are added in setConnection method depending on database version
        partitionType->setCurrentIndex(0);
        widgets.append(qMakePair(tr("Type"), (QWidget*)partitionType));
        connect(partitionType, SIGNAL(currentIndexChanged(int)), this, SLOT(partitionTypeComboChanged(int)));
    }

    if(configureFor!=OraExp::PartitionPartSubpartitionTemplate){
        columnSelector=new LineEditWithButton(this);
        columnSelector->lineEdit()->setItemListRetriever(tableCreator->getColumnsTab());
        connect(columnSelector->lineEdit(), SIGNAL(textEdited(QString)), this, SLOT(toUpper(QString)));
        connect(columnSelector, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(browseColumnList(LineEditWithButton*)));
        connect(columnSelector->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
        widgets.append(qMakePair(tr("Columns"), (QWidget*)columnSelector));
    }

    specifiedBySelector=new QComboBox();
    specifiedBySelector->addItem(tr("Quantity"));
    specifiedBySelector->addItem(tr("Individual partitions"));
    specifiedBySelector->setCurrentIndex(1);
    widgets.append(qMakePair(tr("Specified by"), (QWidget*)specifiedBySelector));
    connect(specifiedBySelector, SIGNAL(currentIndexChanged(int)), this, SLOT(hashPartitionSpecifiedByChanged(int)));

    QIntValidator *intValidator=new QIntValidator(this);
    partitionCountEditor=new QLineEdit();
    partitionCountEditor->setValidator(intValidator);
    widgets.append(qMakePair(tr("Partition count"), (QWidget*)partitionCountEditor));
    connect(partitionCountEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));

    if(configureFor==OraExp::PartitionPartPartition){
        intervalSelector=new QComboBox();
        intervalSelector->setEditable(true);
        intervalSelector->addItem("");
        intervalSelector->addItem("NUMTOYMINTERVAL(1, 'YEAR')");
        intervalSelector->addItem("NUMTOYMINTERVAL(1, 'MONTH')");
        intervalSelector->addItem("NUMTODSINTERVAL(14, 'DAY')");
        intervalSelector->addItem("NUMTODSINTERVAL(7, 'DAY')");
        intervalSelector->addItem("NUMTODSINTERVAL(1, 'DAY')");
        intervalSelector->addItem("NUMTODSINTERVAL(12, 'HOUR')");
        intervalSelector->addItem("NUMTODSINTERVAL(1, 'HOUR')");
        intervalSelector->addItem("NUMTODSINTERVAL(30, 'MINUTE')");
        intervalSelector->addItem("NUMTODSINTERVAL(1, 'MINUTE')");
        widgets.append(qMakePair(tr("Interval"), (QWidget*)intervalSelector));

        connect(intervalSelector->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));

        foreignKeySelector=new QComboBox(this);
        foreignKeySelector->setEditable(true);

        if(configureForIndex){
            foreignKeySelector->hide();
        }else{
            widgets.append(qMakePair(tr("Foreign key"), (QWidget*)foreignKeySelector));
            connect(foreignKeySelector->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
        }
    }

    storeInSelector=new LineEditWithButton(this);
    connect(storeInSelector->lineEdit(), SIGNAL(textEdited(QString)), this, SLOT(toUpper(QString)));
    connect(storeInSelector, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(browseTablespaceList(LineEditWithButton*)));
    connect(storeInSelector->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));

    widgets.append(qMakePair(tr("Store in"), (QWidget*)storeInSelector));

    layout->addLayout(WidgetHelper::layoutWidgetsInColumns(widgets, 2));

    QStringList columnNames;
    if(configureFor==OraExp::PartitionPartSubpartition){
        columnNames.append(tr("Partition"));
    }
    columnNames.append(configureFor==OraExp::PartitionPartPartition ? tr("Name") : tr("Subpartition name"));
    columnNames.append(tr("Values less than"));
    columnNames.append(tr("Tablespace"));
    PartitionsModel *model=new PartitionsModel(columnNames, configureFor, this);
    table=new DataTableAndToolBarWidget(model);
    table->table()->setEditTriggers(QAbstractItemView::AllEditTriggers);
    table->table()->horizontalHeader()->setDefaultSectionSize(200);

    partitionNameDelegate=new IdentifierNameDelegate(NULL, this);
    table->table()->setItemDelegateForColumn(configureFor==OraExp::PartitionPartPartition ?
                                                 model->PartitionNameColIx : model->SubpartitionNameColIx, partitionNameDelegate);

    QIcon tablespaceIcon=IconUtil::getIcon("tablespace");
    tablespaceDelegate=new ComboBoxDelegate(this, true, tablespaceIcon);
    table->table()->setItemDelegateForColumn(model->TablespaceColIx, tablespaceDelegate);

    layout->addWidget(table);

    connect(table->table()->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));

    setLayout(layout);


    partitionTypeComboChanged(currentPartitionType);
    hashPartitionSpecifiedByChanged(0);

    parentTab=dynamic_cast<TableCreatorPartitions*>(parent);
    if(parentTab==0){
        QMessageBox::information(this->window(), tr("Failed to cast type"), tr("Failed to cast 'parent' to TableCreatorPartitions in constructor of TableCreatorPartitionsTab"));
    }else{
        connect(parentTab, SIGNAL(currentTabActivated()), this, SLOT(partitionsTabActivated()));
    }

    if(configureFor==OraExp::PartitionPartSubpartition){
        model->setTitleColumn(1);
        table->toolBar()->addAction(IconUtil::getIcon("normalize_data"), tr("Normalize subpartition list"), this, SLOT(normalizeTableData()));
    }
}

void TableCreatorPartitionsTab::setConnection(DbConnection *db)
{
    //this->db=db;

    partitionNameDelegate->setConnection(db);

    if(configureFor==OraExp::PartitionPartPartition){

        supportsIntervalAndReferencePartitioning=db->supportsIntervalAndReferencePartitioning();

        if(!configureForIndex){
            partitionType->addItem(tr("Hash"), (int)OraExp::PartitionTypeHash);
        }else if(db->supportsHashPartitioningOnIndexes()){
            partitionType->addItem(tr("Hash"), (int)OraExp::PartitionTypeHash);
        }

        if(supportsIntervalAndReferencePartitioning){
            if(!configureForIndex){
                partitionType->addItem(tr("Reference"), (int)OraExp::PartitionTypeReference);
            }
        }

        partitionTypeComboChanged(currentPartitionType);
    }
}

void TableCreatorPartitionsTab::tablespaceListAvailable()
{
    if(parentTab!=0){
        QStringList tablespaces;
        tablespaces.append("");
        tablespaces.append(parentTab->tablespaceList);
        tablespaceDelegate->setList(tablespaces);
        storeInSelector->lineEdit()->setItemList(parentTab->tablespaceList);
    }
}

void TableCreatorPartitionsTab::setEditMode()
{
    Q_ASSERT(!editMode);

    editMode=true;

    if(partitioningStrategy!=0){partitioningStrategy->setEnabled(false);}
    if(partitionType!=0){partitionType->setEnabled(false);}
    if(columnSelector!=0){columnSelector->setEnabled(false);}
    if(foreignKeySelector!=0){foreignKeySelector->setEnabled(false);}

    if(configureFor!=OraExp::PartitionPartSubpartitionTemplate){
        specifiedBySelector->setEnabled(false);
        storeInSelector->setEnabled(false);
    }

    connect(table->table()->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));
}

void TableCreatorPartitionsTab::setInterval(const QString &interval)
{
    int ix=intervalSelector->findText(interval, Qt::MatchFixedString);
    if(ix==-1){
        ix=1;
        intervalSelector->insertItem(ix, interval);
    }
    intervalSelector->setCurrentIndex(ix);
}


void TableCreatorPartitionsTab::populateTableWithPartitionList(const PartitioningInfo &partitionginInfo, int frozenRowIx)
{
    table->table()->setUpdatesEnabled(false);

    int colCount=partitionginInfo.partitions.count();

    PartitionsModel *model=static_cast<PartitionsModel*>(table->table()->model());
    model->ensureRowCount(colCount);

    PartitionInfo pInfo;
    for(int i=0; i<partitionginInfo.partitions.count(); ++i){
        pInfo = partitionginInfo.partitions.at(i);

        if(model->PartitionNameColIx!=-1){
            model->setData(model->index(i, model->PartitionNameColIx), pInfo.partitionName);
        }
        if(model->SubpartitionNameColIx!=-1){
            model->setData(model->index(i, model->SubpartitionNameColIx), pInfo.subpartitionName);
        }
        model->setData(model->index(i, model->ValuesColIx), pInfo.highValue);
        model->setData(model->index(i, model->TablespaceColIx), pInfo.tablespaceName);

        if(pInfo.markedForDeletion){
            model->markRowAsDeleted(i, true);
        }
    }

    table->table()->resizeColumnsAccountingForEditor();

    table->table()->setUpdatesEnabled(true);

    if(configureFor!=OraExp::PartitionPartSubpartitionTemplate && editMode){
        int lastRowIx=(frozenRowIx==-1 ? model->rowCount()-1 : frozenRowIx);
        model->freezeRow(lastRowIx);
        disableColumnsForFrozenRows(lastRowIx);
    }
}

void TableCreatorPartitionsTab::loadFromPartitioningInfo(const PartitioningInfo &pInfo, int frozenRowIx)
{

    if(partitioningStrategy!=0){partitioningStrategy->setCurrentIndex(pInfo.partitioningStrategy);}
    if(partitionType!=0){partitionType->setCurrentIndex(partitionType->findData((int)pInfo.type));}
    if(columnSelector!=0){columnSelector->lineEdit()->setText(pInfo.columns);}
    if(specifiedBySelector!=0 && pInfo.specifiedBy!=-1){specifiedBySelector->setCurrentIndex(pInfo.specifiedBy);}
    if(partitionCountEditor!=0){partitionCountEditor->setText(getEditorStrValue(pInfo.partitionCount));}
    if(intervalSelector!=0){intervalSelector->lineEdit()->setText(pInfo.interval);}
    if(foreignKeySelector!=0){foreignKeySelector->lineEdit()->setText(pInfo.foreignKey);}
    if(storeInSelector!=0){storeInSelector->lineEdit()->setText(pInfo.storeIn);}

    if(pInfo.partitions.size()>0){
        populateTableWithPartitionList(pInfo, frozenRowIx);
    }
}

PartitioningInfo TableCreatorPartitionsTab::partitioningInfoFromWidgets(bool includeIncorrectRows) const
{
    PartitioningInfo info;

    if(partitioningStrategy!=0){info.partitioningStrategy=partitioningStrategy->currentIndex();}
    if(partitionType!=0){info.type=(OraExp::PartitionType)partitionType->itemData(partitionType->currentIndex()).toInt();}
    if(columnSelector!=0){info.columns=columnSelector->lineEdit()->text().trimmed().toUpper();}
    if(specifiedBySelector!=0){info.specifiedBy=specifiedBySelector->currentIndex();}
    if(partitionCountEditor!=0){info.partitionCount=getEditorIntValue(partitionCountEditor->text());}
    if(intervalSelector!=0){info.interval=intervalSelector->lineEdit()->text().trimmed();}
    if(foreignKeySelector!=0){info.foreignKey=foreignKeySelector->lineEdit()->text().trimmed().toUpper();}
    if(storeInSelector!=0){info.storeIn=storeInSelector->lineEdit()->text().trimmed().toUpper();}

    PartitionsModel *model=static_cast<PartitionsModel*>(table->table()->model());
    int rowCount=model->rowCount();

    PartitionInfo pInfo;
    for(int i=0; i<rowCount; ++i){
        pInfo=model->itemInfoFromModelRow(i);
        if(pInfo.partitionPosition!=-1 || includeIncorrectRows){
            info.partitions.append(pInfo);
        }
    }

    return info;
}

void TableCreatorPartitionsTab::disableColumnsForFrozenRows(int rowIx)
{
    PartitionsModel *model=static_cast<PartitionsModel*>(table->table()->model());
    if(configureFor==OraExp::PartitionPartSubpartition){
        model->setColumnEnabled(model->PartitionNameColIx, false, rowIx);
    }

    model->setColumnEnabled(model->TablespaceColIx, false, rowIx);

    if(currentPartitionType!=OraExp::PartitionTypeList){
        model->setColumnEnabled(model->ValuesColIx, false, rowIx);
    }
}

void TableCreatorPartitionsTab::setPartitionNameRetriever(IStringListRetriever *partitionNameRetriever)
{
    ColumnSelectorDelegate *partitionSelectorDelegate=new ColumnSelectorDelegate(partitionNameRetriever, tr("Select partition"), this, true);
    table->table()->setItemDelegateForColumn(0, partitionSelectorDelegate);
}

QStringList TableCreatorPartitionsTab::getStringList(const QString &)
{
    QStringList items;

    PartitionsModel *model=static_cast<PartitionsModel*>(table->table()->model());
    int rowCount=model->rowCount();
    QString partName;
    for(int i=0; i<rowCount; ++i){
        partName=model->index(i, model->PartitionNameColIx).data().toString().trimmed();
        if(!partName.isEmpty() && !model->isRowDeleted(i) && model->isRowDataCorrect(i).isEmpty()){
            items.append(partName);
        }
    }

    return items;
}

int TableCreatorPartitionsTab::getPartitioningStrategy() const
{
    return partitioningStrategy->currentIndex();
}

void TableCreatorPartitionsTab::setPartitioningStrategy(int strategyIx, bool enableControl)
{
    partitioningStrategy->setCurrentIndex(strategyIx);
    partitioningStrategy->setEnabled(enableControl);
}

OraExp::PartitionType TableCreatorPartitionsTab::getPartitionType() const
{
    return currentPartitionType;
}

void TableCreatorPartitionsTab::setPartitionType(OraExp::PartitionType pType)
{
    partitionType->setCurrentIndex(partitionType->findData(pType));
}

void TableCreatorPartitionsTab::setPartitionColumns(const QString &columns)
{
    columnSelector->lineEdit()->setText(columns);
}

void TableCreatorPartitionsTab::partitionTypeComboChanged(int selPartitionType)
{
    if(partitionType==0){
        currentPartitionType=(OraExp::PartitionType)selPartitionType;
    }else{
        currentPartitionType=(OraExp::PartitionType)partitionType->itemData(partitionType->currentIndex()).toInt();
    }
    if(columnSelector!=0){
        columnSelector->setEnabled(currentPartitionType!=OraExp::PartitionTypeReference);
    }
    specifiedBySelector->setEnabled(currentPartitionType==OraExp::PartitionTypeHash);
    if(intervalSelector!=0){
        intervalSelector->setEnabled(currentPartitionType==OraExp::PartitionTypeRange && supportsIntervalAndReferencePartitioning);
    }
    if(foreignKeySelector!=0){
        foreignKeySelector->setEnabled(currentPartitionType==OraExp::PartitionTypeReference);
    }

    hashPartitionSpecifiedByChanged(specifiedBySelector->currentIndex());

    emit partitionTypeChanged((int)currentPartitionType);
}

void TableCreatorPartitionsTab::hashPartitionSpecifiedByChanged(int specifiedByIx)
{
    partitionCountEditor->setEnabled(currentPartitionType==OraExp::PartitionTypeHash && specifiedByIx==0);
    storeInSelector->setEnabled((currentPartitionType==OraExp::PartitionTypeHash && specifiedByIx==0)||
                                (currentPartitionType==OraExp::PartitionTypeRange && supportsIntervalAndReferencePartitioning));
    adjustTable();

    emit ddlChanged();
}

void TableCreatorPartitionsTab::adjustTable()
{
    int specifiedByIx=specifiedBySelector->currentIndex();

    bool disable = (currentPartitionType==OraExp::PartitionTypeReference) ||
                   (currentPartitionType==OraExp::PartitionTypeHash && specifiedByIx==0);
    table->setEnabled(!disable);

    PartitionsModel *model=static_cast<PartitionsModel*>(table->table()->model());
    if(currentPartitionType==OraExp::PartitionTypeList){
        model->setHeaderData(model->ValuesColIx, Qt::Horizontal, tr("Values"));
    }else{
        model->setHeaderData(model->ValuesColIx, Qt::Horizontal, tr("Values less than"));
    }

    table->table()->setColumnHidden(model->ValuesColIx, currentPartitionType==OraExp::PartitionTypeHash);
    model->setCurrentPartitionType(currentPartitionType);
}

void TableCreatorPartitionsTab::toUpper(const QString &text)
{
    QLineEdit *lineEdit= qobject_cast<QLineEdit *>(sender());
    if (!lineEdit){
        return;
    }
    int cursorPosition=lineEdit->cursorPosition();
    lineEdit->setText(text.toUpper());
    lineEdit->setCursorPosition(cursorPosition);
}

void TableCreatorPartitionsTab::browseColumnList(LineEditWithButton *lineEditWithButton)
{
    ColumnSelectorDialog dialog(lineEditWithButton->lineEdit()->getItemListRetriever()->getStringList(),
                                splitTrimmed(lineEditWithButton->lineEdit()->text()),
                                this->window());
    if(dialog.exec()){
        lineEditWithButton->lineEdit()->setText(dialog.getSelectedColumnsCommaSeparated());

        emit ddlChanged();
    }
}

void TableCreatorPartitionsTab::browseTablespaceList(LineEditWithButton *lineEditWithButton)
{
    try{
        QStringList tablespaces;

        if(parentTab!=0){
            tablespaces=parentTab->tablespaceList;
        }

        ColumnSelectorDialog dialog(tablespaces,
                                    splitTrimmed(lineEditWithButton->lineEdit()->text()),
                                    this->window());
        dialog.setWindowTitle(tr("Select tablespaces"));
        if(dialog.exec()){
            lineEditWithButton->lineEdit()->setText(dialog.getSelectedColumnsCommaSeparated());

            emit ddlChanged();
        }

    }catch(OciException &ex){
        QMessageBox::critical(this->window(), tr("Error while getting tablespace list"),
                              tr("Following error occured while getting tablespace list: %1").arg(ex.getErrorMessage()));
    }
}


void TableCreatorPartitionsTab::partitionsTabActivated()
{
    if(foreignKeySelector==0){
        return;
    }

    QString currentKey=foreignKeySelector->currentText();
    foreignKeySelector->clear();
    foreignKeySelector->addItems(foreignKeyRetriever->getStringList("fk"));
    WidgetHelper::setComboBoxText(foreignKeySelector, currentKey);
}

QString TableCreatorPartitionsTab::generateDdl() const
{
    return partitioningInfoFromWidgets(false).generateDdl(configureFor);
}

QString TableCreatorPartitionsTab::generatePartitionListDdl(TableCreatorPartitionsTab *subpartitionsTab, const QString &parentPartitionName) const
{
    if(!table->isEnabled()){
        return "";
    }

    PartitioningInfo *subpartitionsInfo=0;
    if(subpartitionsTab!=0){
        *subpartitionsInfo=subpartitionsTab->partitioningInfoFromWidgets(false);
    }
    return partitioningInfoFromWidgets(false).generatePartitionListDdl(configureFor,
                                                                       subpartitionsInfo,
                                                                       parentPartitionName);
}

void TableCreatorPartitionsTab::alterQuerySucceeded(const QString &taskName, PartitioningInfo &partitionginInfo)
{
    PartitioningInfo modifiedPInfo=partitioningInfoFromWidgets(false);
    int rowIx=partitionginInfo.commitChanges(taskName, modifiedPInfo);

    if(rowIx!=-1){

        PartitionsModel *model=static_cast<PartitionsModel*>(table->table()->model());
        if(rowIx > model->getFrozenRow()){
            model->freezeRow(rowIx);
            disableColumnsForFrozenRows(rowIx);
        }

        ItemCreatorHelper::markDataChanges(model, rowIx, &partitionginInfo.partitions);
    }
}

void TableCreatorPartitionsTab::tableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    PartitionsModel *model=static_cast<PartitionsModel*>(table->table()->model());
    ItemCreatorHelper::handleTableDataChanged(model, &parentTab->getPartitionPartInfo(configureFor)->partitions, from, to);
}

void TableCreatorPartitionsTab::removeIncorrectRows()
{
    WidgetHelper::removeIncorrectRows(table->table());
}

bool TableCreatorPartitionsTab::normalizeTableData(bool prompt)
{
    if(configureFor!=OraExp::PartitionPartSubpartition){
        return false;
    }

    PartitionsModel *model=static_cast<PartitionsModel*>(table->table()->model());
    ColumnSelectorDelegate *colDelegate=static_cast<ColumnSelectorDelegate*>(table->table()->itemDelegateForColumn(model->PartitionNameColIx));
    Q_ASSERT(colDelegate);

    QString partitionName;
    QList< QPair<int,QString> > rowsToNormalize;

    for(int i=model->getFrozenRow()+1; i<model->rowCount(); ++i){
        if(!model->isRowDataCorrect(i).isEmpty()){
            continue;
        }

        partitionName=model->data(model->index(i,model->PartitionNameColIx)).toString().trimmed().toUpper();
        if(partitionName.isEmpty() || partitionName.contains(',')){
            rowsToNormalize.append(qMakePair(i, partitionName));
        }
    }

    if(prompt && rowsToNormalize.size()>0){
        if(QMessageBox::question(this->window(),
                                  tr("Normalize subpartition list"), tr("Subpartition list contains records that need to be normalized. Normalize them now?"),
                                  QMessageBox::Ok, QMessageBox::Cancel)!=QMessageBox::Ok){
            return false;
        }
    }

    int rowIx;
    QStringList allPartitionNames=colDelegate->getColumnNameRetriever()->getStringList();
    QStringList partitionNames;

    QString subpartitionName;
    QString highValue;
    QString tablespaceName;

    table->table()->setUpdatesEnabled(false);
    int shiftOffset=0;

    for(int i=0; i<rowsToNormalize.size(); ++i){
        rowIx=rowsToNormalize.at(i).first+shiftOffset;
        partitionName=rowsToNormalize.at(i).second;

        if(partitionName.isEmpty()){
            partitionNames=allPartitionNames;
        }else{
            partitionNames=splitTrimmed(partitionName);
        }

        if(partitionNames.isEmpty()){
            continue;
        }

        subpartitionName=model->data(model->index(rowIx,model->SubpartitionNameColIx)).toString().trimmed().toUpper();
        highValue=model->data(model->index(rowIx,model->ValuesColIx)).toString().trimmed();
        tablespaceName=model->data(model->index(rowIx,model->TablespaceColIx)).toString().trimmed().toUpper();

        model->removeRow(rowIx);
        model->insertRows(rowIx, partitionNames.size());

        for(int k=0; k<partitionNames.size(); ++k){
            model->setData(model->index(rowIx+k,model->PartitionNameColIx), partitionNames.at(k));
            if(!subpartitionName.isEmpty()){
                model->setData(model->index(rowIx+k,model->SubpartitionNameColIx), QString("%1_%2").arg(partitionNames.at(k), subpartitionName));
            }
            model->setData(model->index(rowIx+k,model->ValuesColIx), highValue);
            model->setData(model->index(rowIx+k,model->TablespaceColIx), tablespaceName);
        }

        shiftOffset+=partitionNames.size()-1;
    }

    table->table()->setUpdatesEnabled(true);

    return true;
}
