#include "dbobjectcreator.h"
#include "connectivity/dbconnection.h"
#include "connectivity/ociexception.h"
#include "connection_page/dbuimanager.h"
#include "dialogs/descriptiveerrordialog.h"
#include "dbobjectcreatorpane.h"
#include "dbobjectcreatorbottompane.h"
#include "connectivity/statement.h"
#include "util/dbutil.h"
#include <QtGui>

DbObjectCreator::DbObjectCreator(const QString &schemaName,
                                 const QString &objectName,
                                 DbUiManager *uiManager,
                                 DbTreeModel::DbTreeNodeType objectType,
                                 CreatorMode creatorMode, QWidget *parent) :
    ConnectionPageTab(uiManager, parent),
    schemaName(schemaName),
    objectName(objectName),
    objectType(objectType),
    creatorMode(creatorMode)
{
}

void DbObjectCreator::createUi()
{
    QVBoxLayout *layout=new QVBoxLayout();

    splitter=new QSplitter(Qt::Vertical);

    creatorPane=createCreatorPane(this);
    creatorPane->createUi();
    splitter->addWidget(creatorPane);

    bottomPane=new DbObjectCreatorBottomPane();
    splitter->addWidget(bottomPane);
    bottomPane->setVisible(false);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(splitter);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(createOrAlterObject()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(cancel()));

    QPushButton *showDdlButton=new QPushButton(tr("Show &DDL"));
    showDdlButton->setCheckable(true);
    showDdlButton->setChecked(false);
    buttonBox->addButton(showDdlButton, QDialogButtonBox::HelpRole);
    buttonBox->layout()->setContentsMargins(5, 5, 5, 5);
    connect(showDdlButton, SIGNAL(clicked(bool)), this, SLOT(ddlViewerToggled(bool)));
    layout->addWidget(buttonBox);

    setLayout(layout);

    connect(creatorPane, SIGNAL(ddlChanged()), this, SLOT(updateDdlText()));
    connect(creatorPane, SIGNAL(objectInfoLoaded()), this, SLOT(emitInitCompletedSignal()));
}

void DbObjectCreator::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    creatorPane->setQueryScheduler(this);
}

void DbObjectCreator::updateDdlText(bool force)
{
    if(bottomPane->isVisible() || force){
        if(creatorMode == EditExisting){
            bottomPane->setEditorText("");
            QList< QueryListItem > ddlList = creatorPane->generateAlterDdl();
            QueryListItem item;
            for(int k=0; k<ddlList.size(); ++k){
                item = ddlList.at(k);
                for(int i=0; i<item.queries.size(); ++i){
                    bottomPane->appendToEditor(QString("%1;\n").arg(item.queries.at(i).second));
                }
            }
        }else{
            bottomPane->setEditorText(creatorPane->generateCreateDdl());
        }
    }
}

void DbObjectCreator::ddlViewerToggled(bool visible)
{
    bottomPane->setVisible(visible);
    if(visible){
        updateDdlText();
    }
}

void DbObjectCreator::createOrAlterObject()
{
    updateDdlText(true);
    if(creatorMode == EditExisting){
        alterObject();
    }else{
        createObject();
    }
}

void DbObjectCreator::createObject()
{
    if(!creatorPane->beforeCreate()){
        return;
    }

    createDdlList=bottomPane->getDdl();

    if(createDdlList.isEmpty()){
        QMessageBox::information(this->window(),
                                 tr("Definition incomplete"),
                                 tr("Definition of object is not complete"));
        return;
    }

    alterQueryIx=0;

    startProgress();

    executeNextCreateQuery();
}

void DbObjectCreator::executeNextCreateQuery()
{
    if(alterQueryIx==createDdlList.size()){ //all queries completed successfully
       updateDdlText();
       stopProgress();

       uiManager->createViewer(creatorPane->getSchemaName().toUpper(), creatorPane->getObjectName().toUpper(), this->objectType);
       uiManager->closeTab(this);

       return;
    }

    this->enqueueQuery(QString("$%1").arg(createDdlList.at(alterQueryIx)),
                       QList<Param*>(),
                       this,
                       "create_database_object",
                       "createQueryCompleted");
}

void DbObjectCreator::createQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        processCreateError(result.exception);
    }else{
        delete result.statement;
        ++alterQueryIx;
        executeNextCreateQuery();
    }
}

void DbObjectCreator::processCreateError(const OciException &ex)
{
    stopProgress();

    QString messageText=tr("Following error occured while creating %1:\n%2").arg(DbUtil::getDbObjectTypeNameByNodeType(this->objectType), ex.getErrorMessage());

    if(alterQueryIx!=0){ //other than first statement failed
        messageText.append("\n");
        messageText.append(tr("Remaining queries will now be opened in a new worksheet\n so that you can manually fix errors and continue."));
    }

    DescriptiveErrorDialog::showMessage(tr("Error creating %1").arg(DbUtil::getDbObjectTypeNameByNodeType(this->objectType)),
                                        ex,
                                        createDdlList.at(alterQueryIx),
                                        this->window(),
                                        messageText);
    if(alterQueryIx!=0){
        QString remainingQueries;
        for(int i=alterQueryIx; i<createDdlList.size(); ++i){
            remainingQueries.append(createDdlList.at(i).trimmed());

            if(i!=createDdlList.size()-1){
                remainingQueries.append("\n");
            }
        }
        uiManager->addWorksheet(remainingQueries);
    }

    createDdlList.clear();
}

void DbObjectCreator::alterObject()
{
    if(!creatorPane->beforeAlter()){
        return;
    }

    startProgress();

    QList<QueryListItem> ddlList = creatorPane->generateAlterDdl();

    alterDdlQueue.clear();

    //normalize
    for(int i=0; i<ddlList.size(); ++i){
        const QueryListItem &item=ddlList.at(i);
        for(int k=0; k<item.queries.size(); ++k){
            const NameQueryPair &pair=item.queries.at(k);

            alterDdlQueue.enqueue(makeTriple(QPointer<QObject>(item.requester), pair.first, pair.second));
        }
    }

    alterQueryIx=0;

    executeNextAlterQuery();
}

void DbObjectCreator::executeNextAlterQuery()
{
    if(alterDdlQueue.isEmpty()){
       updateDdlText();
       stopProgress();
       return;
    }

    currentAlterItem = alterDdlQueue.dequeue();

    this->enqueueQuery(QString("$%1").arg(currentAlterItem.third),
                       QList<Param*>(),
                       this,
                       currentAlterItem.second,
                       "alterQueryCompleted");
}

void DbObjectCreator::alterQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        processAlterError(result.exception);
    }else{
        delete result.statement;

        processAlterSuccess();
    }

    executeNextAlterQuery();
}

void DbObjectCreator::processAlterSuccess()
{
    bool invokeResult=QMetaObject::invokeMethod(currentAlterItem.first, "alterQuerySucceeded",
                              Qt::DirectConnection, Q_ARG(QString, currentAlterItem.second));
    if(!invokeResult){
        qDebug() << "Could not invoke alterQuerySucceeded slot on" << currentAlterItem.first << "for task named"<<currentAlterItem.second;
    }
}

void DbObjectCreator::processAlterError(const OciException &ex)
{
    if(currentAlterItem.first){
        bool invokeResult=QMetaObject::invokeMethod(currentAlterItem.first, "alterQueryError",
                                  Qt::DirectConnection, Q_ARG(QString, currentAlterItem.second));
        if(!invokeResult){
            qDebug() << "Could not invoke alterQueryError slot on" << currentAlterItem.first << "for task named"<<currentAlterItem.second;
        }
    }

    QString messageText=ex.getErrorMessage();
    DescriptiveErrorDialog::showMessage(tr("Error altering %1").arg(DbUtil::getDbObjectTypeNameByNodeType(this->objectType)),
                                        ex,
                                        currentAlterItem.third,
                                        this->window(),
                                        messageText);

    alterDdlQueue.clear();
}

void DbObjectCreator::cancel()
{
    int confirmed=QMessageBox::question(this->window(),
                                        tr("Confirm cancellation"),
                                        tr("Do you really want to close this tab?"),
                                        QMessageBox::Yes, QMessageBox::No);
    if(confirmed==QMessageBox::Yes){
        uiManager->closeTab(this);
    }
}

void DbObjectCreator::startProgress()
{
    this->setEnabled(false);
}

void DbObjectCreator::stopProgress()
{
    this->setEnabled(true);
}

