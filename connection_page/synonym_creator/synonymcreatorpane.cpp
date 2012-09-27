#include "synonymcreatorpane.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/nameeditor.h"
#include "util/strutil.h"
#include "beans/synonyminfo.h"
#include "interfaces/iqueryscheduler.h"
#include "connectivity/dbconnection.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"
#include <QtGui>

SynonymCreatorPane::SynonymCreatorPane(DbObjectCreator *objectCreator, QWidget *parent) :
    DbObjectCreatorSimplePane(objectCreator, parent)
{

}

void SynonymCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectCreatorSimplePane::setQueryScheduler(queryScheduler);

    if(queryScheduler->getDb()->getSchemaName()!=schemaList->currentText()
            //|| is public synonym. either disable here or generate recreate script
            ){
        synonymNameEditor->setEnabled(false);
    }
}

QLayout *SynonymCreatorPane::createForm()
{
    QFormLayout *form=new QFormLayout();

    schemaList=new DbItemListComboBox(objectCreator->getOriginalSchemaName(), "user", true);
    form->addRow(tr("Schema"), schemaList);

    synonymNameEditor=new NameEditor();
    form->addRow(tr("Synonym name"), synonymNameEditor);

    return form;
}

QString SynonymCreatorPane::getSchemaName() const
{
    return schemaList->lineEdit()->text().trimmed().toUpper();
}

QString SynonymCreatorPane::getObjectName() const
{
    return synonymNameEditor->text().trimmed().toUpper();
}

QString SynonymCreatorPane::generateCreateDdl()
{
    return "";
}

QList<QueryListItem> SynonymCreatorPane::generateAlterDdl()
{
    return QList<QueryListItem>();
}

void SynonymCreatorPane::alterQuerySucceeded(const QString &taskName)
{
    SynonymInfo *originalSynonymInfo=getOriginalObjectInfo<SynonymInfo>();
    Q_ASSERT(originalSynonymInfo);
}

SynonymInfo SynonymCreatorPane::getSynonymInfo() const
{
    SynonymInfo info;

    return info;
}

void SynonymCreatorPane::setObjectInfo(DbObjectInfo *objectInfo)
{
    SynonymInfo *synonymInfo=static_cast<SynonymInfo*>(objectInfo);
    Q_ASSERT(synonymInfo);
}

void SynonymCreatorPane::disableControlsForEditMode()
{
    schemaList->setEnabled(false);
}
