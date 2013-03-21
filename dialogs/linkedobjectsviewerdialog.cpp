#include "linkedobjectsviewerdialog.h"
#include "widgets/datatable.h"
#include "util/dialoghelper.h"
#include "util/iconutil.h"
#include <QtGui>

LinkedObjectsViewerDialog::LinkedObjectsViewerDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Linked objects"));

    QVBoxLayout *mainLayout=new QVBoxLayout();

    tableModel = new QStandardItemModel(0, 2, this);
    tableModel->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Type") << tr("Level") << tr("Reference type"));
    table = new DataTable();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(tableModel);
    table->sortByColumn(2);
    table->setSortingEnabled(true);

    mainLayout->addWidget(table);

    QDialogButtonBox *buttonBox=DialogHelper::createButtonBox(this);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Select in Tree View"));
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    resize(700, 500);
}

void LinkedObjectsViewerDialog::setObjectList(const QStringList &objectNames, const QStringList &objectTypes, const QStringList &levels, const QStringList &refTypes)
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
        QStandardItem *refTypeItem = new QStandardItem(refTypes.at(i));

        tableModel->appendRow(QList<QStandardItem*>() << objNameItem << objTypeItem << levelItem << refTypeItem);
    }

    tableModel->sort(2);
    table->resizeColumnsToContents();
    if(table->horizontalHeader()->sectionSize(3)>200){
        table->horizontalHeader()->resizeSection(3, 200);
    }
    //table->horizontalHeader()->setStretchLastSection(true);
}
