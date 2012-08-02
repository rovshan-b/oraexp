#ifndef EXTERNALTABLEFIELDSMODEL_H
#define EXTERNALTABLEFIELDSMODEL_H

#include "genericeditabletablemodel.h"

class ExternalTableFieldsModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit ExternalTableFieldsModel(const QStringList &headerTitles, QObject *parent);

    enum ColumnOrder
    {
        FieldName,
        FieldPosSpec,
        FieldDataType,
        FieldTerminatedBy,
        FieldEnclosedBy,
        FieldTrim,
        FieldDateType,
        FieldDateMask,
        FieldNullIf,
        FieldDefaultIf
    };

    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
    virtual QVariant data ( const QModelIndex & index, int role) const;

private:
    bool isColumnEnabled(const QModelIndex &index) const;

};

#endif // EXTERNALTABLEFIELDSMODEL_H
