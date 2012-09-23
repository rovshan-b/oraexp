#include "dbobjectcreatorsimplepane.h"
#include "dbobjectcreator.h"
#include "util/iconutil.h"
#include "metadata_loaders/metadataloader.h"
#include "metadata_loaders/metadataloaderfactory.h"
#include "widgets/dbitemlistcombobox.h"
#include <QtGui>

DbObjectCreatorSimplePane::DbObjectCreatorSimplePane(DbObjectCreator *objectCreator, QWidget *parent) :
    DbObjectCreatorPane(objectCreator, parent), originalObjectInfo(0)
{
    editMode=!objectCreator->getOriginalObjectName().isEmpty();
}

DbObjectCreatorSimplePane::~DbObjectCreatorSimplePane()
{
    delete originalObjectInfo;
}

void DbObjectCreatorSimplePane::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    QWidget *formContainerWidget=new QWidget();
    formContainerWidget->setMinimumWidth(350);

    QLayout *form=createForm();

    formContainerWidget->setLayout(form);

    QScrollArea *scrollArea=new QScrollArea();
    scrollArea->setContentsMargins(0,0,0,0);
    scrollArea->setWidget(formContainerWidget);
    //scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(scrollArea);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    if(editMode){
        disableControlsForEditMode();
    }
}

void DbObjectCreatorSimplePane::objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader)
{
    Q_ASSERT(originalObjectInfo==0);
    originalObjectInfo=objectInfo;

    setObjectInfo(originalObjectInfo);

    loader->deleteLater();

    emit objectInfoLoaded();
}

void DbObjectCreatorSimplePane::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    QMessageBox::critical(this, tr("Failed to load object information"),
                          tr("Task name: %1\nError: %2").arg(taskName, ex.getErrorMessage()));

    loader->deleteLater();
}

void DbObjectCreatorSimplePane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectCreatorPane::setQueryScheduler(queryScheduler);

    DbItemListComboBox *schemaList=schemaListCombo();
    if(schemaList){
        if(!schemaList->isEnabled()){
            schemaList->addItem(IconUtil::getIcon("user"), schemaList->currentText());
            schemaList->setCurrentIndex(0);
        }else{
            schemaList->loadItems(this->queryScheduler, "get_schema_list");
        }
    }

    if(editMode){
        MetadataLoader *metadataLoader=MetadataLoaderFactory::createMetadataLoader(objectCreator->getObjectType(),
                                                    this->queryScheduler,
                                                    objectCreator->getOriginalSchemaName(),
                                                    objectCreator->getOriginalObjectName(),
                                                    "", false, this);
        Q_ASSERT(metadataLoader);
        connect(metadataLoader, SIGNAL(objectInfoReady(DbObjectInfo*,MetadataLoader*)), this, SLOT(objectInfoReady(DbObjectInfo*,MetadataLoader*)));
        connect(metadataLoader, SIGNAL(loadError(QString,OciException,MetadataLoader*)), this, SLOT(loadError(QString,OciException,MetadataLoader*)));
        metadataLoader->loadObjectInfo();
    }else{
        emit objectInfoLoaded();
    }
}
