#include "compilermessagespane.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include <QtGui>

CompilerMessagesPane::CompilerMessagesPane(QWidget *parent) :
    InfoPane(parent)
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
}

void CompilerMessagesPane::addCompilerMessage(int line, int position,
                                              const QString &message, const QString &attribute)
{
    QStandardItem *lineItem=new QStandardItem(QString::number(line));
    QStandardItem *positionItem=new QStandardItem(QString::number(position));
    QStandardItem *messageItem=new QStandardItem(message);
    messageItem->setIcon(IconUtil::getIcon(attribute=="ERROR" ? "help" : "warning"));

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
