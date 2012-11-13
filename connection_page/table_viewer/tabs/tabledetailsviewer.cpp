#include "tabledetailsviewer.h"
#include "util/queryutil.h"
#include "util/strutil.h"
#include "connectivity/dbconnection.h"
#include "widgets/datatable.h"
#include <QtGui>

#include <iostream>
using namespace std;

TableDetailsViewer::TableDetailsViewer(QWidget *parent) :
    DbObjectViewerGenericTab("", parent)
{

}

void TableDetailsViewer::createMainWidget(QLayout *layout)
{
    scrollArea=new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setBackgroundRole(QPalette::Base);
    layout->addWidget(scrollArea);
}

void TableDetailsViewer::loadData()
{
    QList<Param*> params;
    params.append(new Param(":owner", schemaName));
    params.append(new Param(":object_name", tableName));

    queryScheduler->enqueueQuery("get_table_details_for_detailed_view", params, this, "get_table_details_for_detailed_view",
                     "tableDetailsQueryCompleted", "tableDetailsRecordAvailable", "tableDetailsFetched");
}

void TableDetailsViewer::tableDetailsQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error retrieving table details"), result.exception.getErrorMessage());
        queryCompleted();
        return;
    }
}

void TableDetailsViewer::tableDetailsRecordAvailable(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching table details"), fetchResult.exception.getErrorMessage());
        queryCompleted();
        return;
    }

    if(labels.isEmpty()){
        createLabels(fetchResult);
    }else{
        setLabelTexts(fetchResult);
    }
}

void TableDetailsViewer::tableDetailsFetched(const QString &)
{
    queryCompleted();
}

void TableDetailsViewer::createLabels(const FetchResult &result)
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

void TableDetailsViewer::setLabelTexts(const FetchResult &result)
{
    scrollArea->setUpdatesEnabled(false);
    for(int i=0; i<result.oneRow.count(); ++i){
        if(i<labels.count()){
            labels.at(i)->setText(result.oneRow.at(i).trimmed());
        }
    }
    scrollArea->setUpdatesEnabled(true);
}
