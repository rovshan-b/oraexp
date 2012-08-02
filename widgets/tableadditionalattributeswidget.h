#ifndef TABLEADDITIONALATTRIBUTESWIDGET_H
#define TABLEADDITIONALATTRIBUTESWIDGET_H

#include <QWidget>
#include "beans/tableadditionalattributes.h"
#include "enums.h"

class QComboBox;
class QLineEdit;
class QCheckBox;
class ComboBoxAndLineEditWidget;
class DbConnection;

class TableAdditionalAttributesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableAdditionalAttributesWidget(DbConnection *db, OraExp::TableType tableType, bool isEditMode, QWidget *parent = 0);

    TableAdditionalAttributes getAttributes() const;
    void setAttributes(const TableAdditionalAttributes &attributes) const;

private:
    QComboBox *parallelComboBox;
    QLineEdit *parallelDegreeEditor;
    QComboBox *loggingComboBox;
    QComboBox *cacheComboBox;
    QComboBox *monitoringComboBox;
    QComboBox *compressComboBox;
    QComboBox *rowDependenciesComboBox;
    QComboBox *rowMovementComboBox;
    ComboBoxAndLineEditWidget *flashbackArchiveEditor;

private slots:
    void enableParallelDegreeEditor();

};

#endif // TABLEADDITIONALATTRIBUTESWIDGET_H
