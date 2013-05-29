#include "tablenamefinderdialog.h"
#include "connection_page/dbuimanager.h"
#include "util/dialoghelper.h"
#include <QtGui>

TableNameFinderDialog::TableNameFinderDialog(const QString &schemaName,
                                             const QString &tabName,
                                             const QString &childObjectName,
                                             DbUiManager *uiManager,
                                             QWidget *parent) :
    ConnectionPageWindow(uiManager, parent),
    schemaName(schemaName),
    tabName(tabName),
    childObjectName(childObjectName)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QProgressBar *progressBar = new QProgressBar();
    progressBar->setMaximumHeight(10);
    progressBar->setRange(0,0);
    mainLayout->addWidget(progressBar);

    setLayout(mainLayout);

    resize(sizeHint());
    DialogHelper::centerWindow(this);

    QTimer::singleShot(1000, this, SLOT(showAndActivate()));
}

void TableNameFinderDialog::setConnection(DbConnection *db)
{
    ConnectionPageWindow::setConnection(db);

    QString childObjectType;
    if(tabName=="Indexes"){
        childObjectType = "INDEX";
    }else if(tabName=="Keys"){
        childObjectType = "CONSTRANT";
    }else{
        Q_ASSERT(false);
    }

    this->enqueueQuery("get_table_name_by_child_object_name",
                       QList<Param*>()
                       << new Param("owner", schemaName)
                       << new Param("object_name", childObjectName)
                       << new Param("object_type", childObjectType),
                       this,
                       "get_table_name_by_child_object_name",
                       "findTableNameQueryCompleted",
                       "findTableNameRecordFetched",
                       "findTableNameFetchCompleted",
                       true);
}

void TableNameFinderDialog::findTableNameQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error retrieving table name"),
                              tr("Task name: %1\nError: %2").arg(result.taskName, result.exception.getErrorMessage()));
        accept();
    }
}

void TableNameFinderDialog::findTableNameRecordFetched(const FetchResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching table name"),
                              tr("Task name: %1\nError: %2").arg(result.taskName, result.exception.getErrorMessage()));
        accept();
        return;
    }

    schemaName = result.colValue("TABLE_OWNER"); //indexes may be in different schema than table
    tableName = result.colValue("TABLE_NAME");
}

void TableNameFinderDialog::findTableNameFetchCompleted(const QString &)
{
    uiManager->addEditor(schemaName, tableName, "TABLE", tabName, childObjectName);
    accept();
}
