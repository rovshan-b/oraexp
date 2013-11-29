#include "constraintselectordelegate.h"
#include "connectivity/dbconnection.h"
#include "connectivity/resultset.h"
#include "connectivity/ociexception.h"
#include "util/widgethelper.h"
#include "util/dbutil.h"
#include "util/iconutil.h"
#include "widgets/dbitemlistcombobox.h"
#include "interfaces/iqueryscheduler.h"
#include "enums.h"
#include <memory>

using namespace std;

ConstraintSelectorDelegate::ConstraintSelectorDelegate(const QString &defaultSchemaName,
                                             int modelSchemaNameColumnIndex,
                                             int modelTableNameColumnIndex,
                                             OraExp::ConstraintType constraintType,
                                             const QString &initialColumnName,
                                             IQueryScheduler *queryScheduler, QObject *parent) :
    ComboBoxDelegate(parent, -1, true),
    defaultSchemaName(defaultSchemaName),
    modelSchemaNameColumnIndex(modelSchemaNameColumnIndex),
    modelTableNameColumnIndex(modelTableNameColumnIndex),
    constraintType(constraintType),
    initialColumnName(initialColumnName),
    queryScheduler(queryScheduler)
{

}

QWidget *ConstraintSelectorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/,
                           const QModelIndex &index) const
{
    QString schemaName=this->defaultSchemaName;

    if(modelSchemaNameColumnIndex!=1){
        QString modelSchemaName=index.model()->data(index.model()->index(index.row(), modelSchemaNameColumnIndex)).toString();
        if(!modelSchemaName.trimmed().isEmpty()){
            schemaName=modelSchemaName;
        }
    }

    QString tableName;

    if(modelTableNameColumnIndex!=-1){
        QString modelTableName=index.model()->data(index.model()->index(index.row(), modelTableNameColumnIndex)).toString();
        if(!modelTableName.trimmed().isEmpty()){
            tableName=modelTableName;
        }
    }

    DbItemListComboBox *editor=new DbItemListComboBox(index.data().toString(), "column_uq", false, false, parent);
    editor->setSilentMode();
    editor->loadItems(queryScheduler, "get_table_constraint_columns",
                      QList<Param*>()
                      //<< new Param(":constraint_type", DbUtil::getConstraintTypeLetter(constraintType))
                      << new Param(":owner", schemaName)
                      << new Param(":object_name", tableName));
    editor->setFrame(false);

    return editor;
}

