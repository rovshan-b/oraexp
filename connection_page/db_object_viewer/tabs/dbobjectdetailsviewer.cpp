#include "dbobjectdetailsviewer.h"
#include "util/queryutil.h"
#include "util/strutil.h"
#include "connectivity/dbconnection.h"
#include "widgets/datatable.h"
#include <QtGui>

DbObjectDetailsViewer::DbObjectDetailsViewer(const QString &queryName, QWidget *parent) :
    DbObjectViewerGenericTab(queryName, parent)
{
}

void DbObjectDetailsViewer::createMainWidget(QLayout *layout)
{
    scrollArea=new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setBackgroundRole(QPalette::Base);
    layout->addWidget(scrollArea);
}

void DbObjectDetailsViewer::loadData()
{
    queryScheduler->enqueueQuery(this->queryName, getQueryParams(), this, this->queryName,
                     "objectDetailsQueryCompleted", "objectDetailsRecordAvailable", "objectDetailsFetched");
}

void DbObjectDetailsViewer::objectDetailsQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error retrieving object details"), result.exception.getErrorMessage());
        queryCompleted();
        return;
    }
}

void DbObjectDetailsViewer::objectDetailsRecordAvailable(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching object details"), fetchResult.exception.getErrorMessage());
        queryCompleted();
        return;
    }

    if(labels.isEmpty()){
        createLabels(fetchResult);
    }else{
        setLabelTexts(fetchResult);
    }
}

void DbObjectDetailsViewer::objectDetailsFetched(const QString &)
{
    queryCompleted();
}

void DbObjectDetailsViewer::createLabels(const FetchResult &result)
{
    //int columnCount = 3;
    //int itemsPerColumn = qCeil((float)result.oneRow.count() / columnCount);
    int itemsPerColumn = 20;
    if(result.oneRow.count()>60){
        itemsPerColumn = 25;
    }

    QWidget *parentWidget=new QWidget();
    QHBoxLayout *mainLayout=new QHBoxLayout();
    QFormLayout *formLayout;
    QLabel *label;
    QString labelTitle;
    for(int i=0; i<result.oneRow.count(); ++i){

        if(i%itemsPerColumn==0){
            formLayout=new QFormLayout();
            mainLayout->addLayout(formLayout);
        }

        label = new QLabel(result.oneRow.at(i).trimmed());
        label->setStyleSheet("font-style:italic");
        label->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
        labelTitle=QString("%1:").arg(humanize(result.columnIndexes.key(i+1)));
        formLayout->addRow(labelTitle, label);

        labels.append(label);
    }

    parentWidget->setLayout(mainLayout);
    scrollArea->setWidget(parentWidget);
}

void DbObjectDetailsViewer::setLabelTexts(const FetchResult &result)
{
    scrollArea->setUpdatesEnabled(false);
    for(int i=0; i<result.oneRow.count(); ++i){
        if(i<labels.count()){
            labels.at(i)->setText(result.oneRow.at(i).trimmed());
        }
    }
    scrollArea->setUpdatesEnabled(true);
}
