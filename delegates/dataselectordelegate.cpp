#include "dataselectordelegate.h"
#include "dialogs/genericresultsetviewerdialog.h"
#include "widgets/lineeditwithbutton.h"

DataSelectorDelegate::DataSelectorDelegate(IQueryScheduler *queryScheduler, const QString &schema, const QString &objectName, const QString &dblinkName, QObject *parent) :
    LineEditWithButtonDelegate(false, parent),
    queryScheduler(queryScheduler),
    schema(schema),
    objectName(objectName),
    dblinkName(dblinkName)
{
    setAutoAppendRows(false);
}

void DataSelectorDelegate::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler = queryScheduler;
}

void DataSelectorDelegate::buttonClicked(LineEditWithButton *senderWidget)
{
    if(!queryScheduler){
        return;
    }

    if(lastQuery.isEmpty()){
        lastQuery = QString("SELECT * FROM \"%1\".\"%2\"").arg(schema, objectName);
        if(!dblinkName.isEmpty()){
            lastQuery.append("@").append(dblinkName);
        }
    }

    GenericResultsetViewerDialog dialog(queryScheduler, lastQuery, QList<Param*>(), "", senderWidget->window(), QPair<QString,QString>(), true);
    dialog.setWindowTitle(tr("Data selector"));
    if(dialog.exec()){
        senderWidget->lineEdit()->setText(dialog.selectedText);

        emit commitData(senderWidget);
    }
    lastQuery = dialog.currentQuery();
}
