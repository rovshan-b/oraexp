#include "dbobjectcompareroptionstab.h"
#include <QtGui>

DbObjectComparerOptionsTab::DbObjectComparerOptionsTab(QWidget *parent) :
    QWidget(parent)
{
}

void DbObjectComparerOptionsTab::targetConnectionEstablished(DbConnection *, DbConnection *)
{
}

bool DbObjectComparerOptionsTab::validate()
{
    return true;
}

QVBoxLayout *DbObjectComparerOptionsTab::createSingleColumnOptionsPane(QVBoxLayout *layout)
{
    QVBoxLayout *optionsPaneLayout=new QVBoxLayout();

    QVBoxLayout *firstColLayout=new QVBoxLayout();
    firstColLayout->setContentsMargins(0,0,0,0);

    optionsPaneLayout->addLayout(firstColLayout);
    optionsPaneLayout->addStretch();

    QWidget *scrollWidget=new QWidget();
    scrollWidget->setContentsMargins(0,0,0,0);
    scrollWidget->setLayout(optionsPaneLayout);

    QScrollArea *scrollArea=new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setBackgroundRole(QPalette::Base);
    scrollArea->setWidget(scrollWidget);

    layout->addWidget(scrollArea);
    layout->setAlignment(optionsPaneLayout, Qt::AlignTop|Qt::AlignLeft);

    return firstColLayout;
}

