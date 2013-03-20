#include "dbobjectselectordialog.h"
#include "widgets/datatable.h"
#include "util/dialoghelper.h"
#include "util/iconutil.h"
#include <QtGui>

DbObjectSelectorDialog::DbObjectSelectorDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Linked objects"));

    QVBoxLayout *mainLayout=new QVBoxLayout();

    tableModel = new QStandardItemModel(0, 2, this);
    tableModel->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Type") << tr("Level"));
    table = new DataTable();
    //table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    //table->setObjectListMode(-1, 0, 1, schemaName);
    table->setModel(tableModel);

    mainLayout->addWidget(table);

    QDialogButtonBox *buttonBox=DialogHelper::createButtonBox(this);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Select in Tree View"));
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    resize(550, 500);
}

void DbObjectSelectorDialog::setObjectList(const QStringList &objectNames, const QStringList &objectTypes, const QStringList &levels)
{
    for(int i=0; i<objectNames.size(); ++i){
        const QString &objectType = objectTypes.at(i);
        QStandardItem *objNameItem = new QStandardItem(IconUtil::getIcon(objectType.toLower().replace(' ', '_')),
                                                      objectNames.at(i));
        objNameItem->setCheckable(true);
        objNameItem->setCheckState(Qt::Checked);

        QStandardItem *objTypeItem = new QStandardItem(objectType);
        QStandardItem *levelItem = new QStandardItem();
        levelItem->setData(levels.at(i).toInt(), Qt::DisplayRole);

        tableModel->appendRow(QList<QStandardItem*>() << objNameItem << objTypeItem << levelItem);
    }

    table->setSortingEnabled(true);
    tableModel->sort(2);
    table->resizeColumnsToContents();
    table->horizontalHeader()->setStretchLastSection(true);
}
