#include "columnselectordialog.h"
#include "ui_columnselectordialog.h"
#include "util/widgethelper.h"
#include <QStandardItemModel>
#include <QStandardItem>

ColumnSelectorDialog::ColumnSelectorDialog(const QStringList &columnList,
                                           const QStringList &selColumnList,
                                           QWidget *parent, bool changeCase) :
    QDialog(parent),
    ui(new Ui::ColumnSelectorDialog),
    allowEmptySelection(false)
{
    ui->setupUi(this);

    allColumnsModel=new QStandardItemModel(ui->allColumnsList);
    allColumnsModel->setSortRole(Qt::UserRole + 1);
    selectedColumnsModel=new QStandardItemModel(ui->selectedColumnsList);

    ui->allColumnsList->setModel(allColumnsModel);
    ui->selectedColumnsList->setModel(selectedColumnsModel);

    int maxButtonWidth=ui->addOneButton->fontMetrics().width(">>>>");
    ui->addOneButton->setMaximumWidth(maxButtonWidth);
    ui->addAllButton->setMaximumWidth(maxButtonWidth);
    ui->removeOneButton->setMaximumWidth(maxButtonWidth);
    ui->removeAllButton->setMaximumWidth(maxButtonWidth);

    ui->allColumnsList->setMinimumWidth(150);
    ui->selectedColumnsList->setMinimumWidth(150);

    QStandardItem *newItem=0;
    int colCount=columnList.size();
    for(int i=0; i<colCount; ++i){
        newItem=new QStandardItem(changeCase ? columnList.at(i).toUpper().trimmed() : columnList.at(i));
        newItem->setData(i, Qt::UserRole + 1); //remember position
        newItem->setEditable(false);
        allColumnsModel->appendRow(newItem);
    }

    int selColCount=selColumnList.count();
    QList<QStandardItem*> foundItems;
    //QItemSelectionModel *selModel=ui->allColumnsList->selectionModel();
    for(int i=0; i<selColCount; ++i){
        foundItems=allColumnsModel->findItems(selColumnList.at(i));
        if(foundItems.count()>0){
            //selModel->select(foundItems.at(0)->index(), QItemSelectionModel::Select);
            selectedColumnsModel->appendRow(allColumnsModel->takeRow(foundItems.at(0)->index().row()));
        }
    }
    //if(selColCount>0){
    //    on_addOneButton_clicked();
    //    selModel->clear();
    //}

    enableOkButton();
}

ColumnSelectorDialog::~ColumnSelectorDialog()
{
    delete ui;
}

void ColumnSelectorDialog::on_addOneButton_clicked()
{
    WidgetHelper::moveSelectedListViewItems(ui->allColumnsList, ui->selectedColumnsList);
    enableOkButton();
}

void ColumnSelectorDialog::on_addAllButton_clicked()
{
    WidgetHelper::moveAllListViewItems(ui->allColumnsList, ui->selectedColumnsList);
    enableOkButton();
}

void ColumnSelectorDialog::on_removeOneButton_clicked()
{
    if(WidgetHelper::moveSelectedListViewItems(ui->selectedColumnsList, ui->allColumnsList)){
        allColumnsModel->sort(0);
        enableOkButton();
    }
}

void ColumnSelectorDialog::on_removeAllButton_clicked()
{
    if(WidgetHelper::moveAllListViewItems(ui->selectedColumnsList, ui->allColumnsList)){
        allColumnsModel->sort(0);
        enableOkButton();
    }
}

void ColumnSelectorDialog::enableOkButton()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(allowEmptySelection || ui->selectedColumnsList->model()->rowCount()>0);
}

QStringList ColumnSelectorDialog::getSelectedColumns() const
{
    QStringList result;
    int selectedCount=selectedColumnsModel->rowCount();
    for(int i=0; i<selectedCount; ++i){
        result.append(selectedColumnsModel->item(i)->text());
    }

    return result;
}

QString ColumnSelectorDialog::getSelectedColumnsCommaSeparated() const
{
    return getSelectedColumns().join(",");
}

void ColumnSelectorDialog::setAllowEmptySelection()
{
    this->allowEmptySelection = true;
}
