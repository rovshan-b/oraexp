#ifndef TABLEADDITIONALATTRIBUTESDIALOG_H
#define TABLEADDITIONALATTRIBUTESDIALOG_H

#include <QDialog>
#include "beans/tableadditionalattributes.h"
#include "enums.h"

class IQueryScheduler;

class TableAdditionalAttributesWidget;

class TableAdditionalAttributesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableAdditionalAttributesDialog(IQueryScheduler *queryScheduler, OraExp::TableType tableType, bool isEditMode, QWidget *parent = 0);

    TableAdditionalAttributes getAttributes() const;
    void setAttributes(const TableAdditionalAttributes &attributes) const;

private:
    TableAdditionalAttributesWidget *attributesWidget;

};

#endif // TABLEADDITIONALATTRIBUTESDIALOG_H
