#include "dbobjectcreator.h"
#include "connectivity/dbconnection.h"
#include "connectivity/ociexception.h"
#include "connection_page/dbuimanager.h"
#include "dialogs/descriptiveerrordialog.h"
#include "dbobjectcreatorpane.h"
#include "dbobjectcreatorbottompane.h"
#include <QtGui>

DbObjectCreator::DbObjectCreator(const QString &schemaName,
                                 const QString &objectName,
                                 DbUiManager *uiManager,
                                 QWidget *parent) :
    ConnectionPageTab(uiManager, parent),
    schemaName(schemaName),
    objectName(objectName)
{
    editMode=!objectName.isEmpty();
}

void DbObjectCreator::createUi()
{
    QVBoxLayout *layout=new QVBoxLayout();

    splitter=new QSplitter(Qt::Vertical);

    creatorPane=createCreatorPane(schemaName, objectName);
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
        if(editMode){
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
    if(editMode){
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

    QStringList ddlList=bottomPane->getDdl();

    if(ddlList.isEmpty()){
        QMessageBox::information(this->window(),
                                 tr("Definition incomplete"),
                                 tr("Definition of table is not complete. Please, make sure to enter table name and column list as a minimum."));
        return;
    }

    QString currentDdl;
    bool success=false;
    int openWorksheetWithQueryIndex=-1;

    for(int i=0; i<ddlList.size(); ++i){
        currentDdl=ddlList.at(i).trimmed();
        if(currentDdl.isEmpty()){
            continue;
        }

        try{
            db->executeQueryAndCleanup(currentDdl);
            success=true;
        }catch(OciException &ex){

            QString messageText=tr("Following error occured while creating table:\n%1").arg(ex.getErrorMessage());

            if(i!=0){ //other than CREATE TABLE statement failed
                messageText.append("\n");
                messageText.append(tr("Remaining queries will now be opened in a new worksheet so that you can manually fix errors and continue."));
            }

            unsigned int errorLine=ex.getErrorRow();
            messageText.append("\nLine: ").append(QString::number(errorLine));
            DescriptiveErrorDialog::showMessage(tr("Error while creating table"),
                                                messageText,
                                                currentDdl,
                                                errorLine,
                                                this->window());

            if(i!=0){ //other than CREATE TABLE statement failed
                openWorksheetWithQueryIndex=i;
            }

            break;
        }
    }

    if(success){
        uiManager->createViewer(creatorPane->getSchemaName().toUpper(), creatorPane->getObjectName().toUpper(),
                                DbTreeModel::Table);
        uiManager->closeTab(this);
    }

    if(openWorksheetWithQueryIndex!=-1){
        QString remainingQueries;
        for(int i=openWorksheetWithQueryIndex; i<ddlList.size(); ++i){
            remainingQueries.append(ddlList.at(i).trimmed());

            if(i!=ddlList.size()-1){
                remainingQueries.append("\n");
            }
        }
        uiManager->addWorksheet(remainingQueries);
    }
}

void DbObjectCreator::alterObject()
{
    if(!creatorPane->beforeAlter()){
        return;
    }

    QList< QueryListItem > ddlList = creatorPane->generateAlterDdl();

    QueryListItem item;
    NameQueryPair pair;
    try{
        for(int k=0; k<ddlList.size(); ++k){
            item = ddlList.at(k);

            for(int i=0; i<item.queries.size(); ++i){
                pair=item.queries.at(i);

                qDebug() << "executing alter task named" << pair.first;
                db->executeQueryAndCleanup(pair.second);

                bool invokeResult=QMetaObject::invokeMethod(item.requester, "alterQuerySucceeded",
                                          Qt::DirectConnection, Q_ARG(QString, pair.first));
                if(!invokeResult){
                    qDebug() << "Could not invoke alterQuerySucceeded slot on" << item.requester << "for task named"<<pair.first;
                }
            }
        }
    }catch(OciException &ex){

        if(item.requester){
            bool invokeResult=QMetaObject::invokeMethod(item.requester, "alterQueryError",
                                      Qt::DirectConnection, Q_ARG(QString, pair.first));
            if(!invokeResult){
                qDebug() << "Could not invoke alterQueryError slot on" << item.requester << "for task named"<<pair.first;
            }
        }

        unsigned int errorLine=ex.getErrorRow();
        QString messageText=ex.getErrorMessage();
        messageText.append("\nLine: ").append(QString::number(errorLine));
        DescriptiveErrorDialog::showMessage(tr("Error altering table"),
                                            messageText,
                                            pair.second,
                                            errorLine,
                                            this->window());
    }

    updateDdlText();
    QMessageBox::information(this->window(),
                             tr("Table altered successfully"),
                             tr("Table altered successfully"));
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

