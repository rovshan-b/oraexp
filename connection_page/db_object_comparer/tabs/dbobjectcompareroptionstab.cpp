#include "dbobjectcompareroptionstab.h"
#include "util/widgethelper.h"
#include <QtGui>

DbObjectComparerOptionsTab::DbObjectComparerOptionsTab(QWidget *parent) :
    QWidget(parent)
{
}

void DbObjectComparerOptionsTab::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    Q_UNUSED(queryScheduler);
}

void DbObjectComparerOptionsTab::targetConnectionEstablished(DbConnection *sourceDb, DbConnection *targetDb)
{
    Q_UNUSED(sourceDb);
    Q_UNUSED(targetDb);
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

    QScrollArea *scrollArea=WidgetHelper::createScrollArea(optionsPaneLayout);

    layout->addWidget(scrollArea);
    layout->setAlignment(optionsPaneLayout, Qt::AlignTop|Qt::AlignLeft);

    return firstColLayout;
}

