#ifndef OBJECTGRANTSMODEL_H
#define OBJECTGRANTSMODEL_H

#include "genericeditabletablemodel.h"
#include "beans/objectgrantinfo.h"
#include "enums.h"

class ObjectGrantsModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit ObjectGrantsModel(const QStringList &headerTitles,
                               OraExp::GrantsEditorPerspective perspective,
                               const QStringList &privilegeNames,
                               DbTreeModel::DbTreeNodeType objectType,
                               QObject *parent);

    enum ColumnOrder
    {
        GrantSchema,
        GrantObject
    };

    QVariant data ( const QModelIndex & index, int role) const;

    virtual QString isRowDataCorrect(int rowIx) const;

    ObjectGrantInfo itemInfoFromModelRow(int row) const;

    QStringList getPrivilegeNames() const {return this->privilegeNames;}

    void setSchemaOrObjectName(const QString &schemaOrObjectName);

    OraExp::GrantsEditorPerspective getPerspective() const {return this->perspective;}

    int offset() const;

private:
    QVariant getGrantTypeText(int grantTypeIx) const;

    OraExp::GrantsEditorPerspective perspective;
    QStringList privilegeNames;
    QString schemaOrObjectName; //based on perspective
    DbTreeModel::DbTreeNodeType objectType;
};

#endif // OBJECTGRANTSMODEL_H
