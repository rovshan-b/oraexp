#include "compilermessagespane.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include <QtGui>

CompilerMessagesPane::CompilerMessagesPane(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);

    table=new DataTable();
    table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainLayout->addWidget(table);

    tableModel=new QStandardItemModel(0, 3, this);
    tableModel->setHorizontalHeaderLabels(QStringList() << tr("Line") << tr("Position") << tr("Message"));
    table->resizeColumnsToFitContents();

    table->setModel(tableModel);

    setLayout(mainLayout);

    connect(table, SIGNAL(activated(QModelIndex)), this, SLOT(tableRowActivated(QModelIndex)));
}

void CompilerMessagesPane::addCompilerMessage(int line, int position,
                                              const QString &message, const QString &attribute)
{
    QStandardItem *lineItem=new QStandardItem(QString::number(line));
    QStandardItem *positionItem=new QStandardItem(QString::number(position));
    QStandardItem *messageItem=new QStandardItem(QString(message).replace("\n", " "));
    messageItem->setIcon(IconUtil::getIcon(attribute=="ERROR" ? "error" : "warning"));

    tableModel->appendRow(QList<QStandardItem*>() << lineItem << positionItem << messageItem);
}

void CompilerMessagesPane::clearCompilerMessages()
{
    tableModel->setRowCount(0);
}

bool CompilerMessagesPane::isEmpty() const
{
    return tableModel->rowCount()==0;
}

void CompilerMessagesPane::resizeToFit()
{
    table->resizeColumnsToFitContents();
    table->horizontalHeader()->setStretchLastSection(true);
}

void CompilerMessagesPane::tableRowActivated(const QModelIndex &index)
{
    QStandardItem *item=tableModel->itemFromIndex(index);
    if(!item){
        return;
    }
    int line=tableModel->item(item->row(), 0)->text().toInt();
    int pos=tableModel->item(item->row(), 1)->text().toInt();
    QString message = tableModel->item(item->row(), 2)->text();

    emit activated(line, pos, message);

}
