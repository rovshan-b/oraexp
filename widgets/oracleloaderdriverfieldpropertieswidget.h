#ifndef ORACLELOADERDRIVERFIELDPROPERTIESWIDGET_H
#define ORACLELOADERDRIVERFIELDPROPERTIESWIDGET_H

#include <QWidget>
#include "interfaces/istringlistretriever.h"

class ComboBoxAndLineEditWidget;
class QLineEdit;
class QCheckBox;
class QComboBox;
class DataTableAndToolBarWidget;
class LineEditAndCheckBoxWidget;
class IQueryScheduler;
class QTabWidget;

class OracleLoaderDriverFieldPropertiesWidget : public QWidget, IStringListRetriever
{
    Q_OBJECT

    friend class FieldNameRetriever;
public:
    explicit OracleLoaderDriverFieldPropertiesWidget(IQueryScheduler *queryScheduler, IStringListRetriever *columnListRetriever, QWidget *parent = 0);

    QString generateDdl() const;
    QString generateFieldDefinitionsDdl() const;
    QString generateColumnTransformsDdl() const;

    void directoryListAvailable(const QStringList &directoryList){this->directoryList=directoryList;}

    class FieldNameRetriever : public IStringListRetriever
    {
    public:
        explicit FieldNameRetriever(OracleLoaderDriverFieldPropertiesWidget *outer) : outer(outer){}

        virtual QStringList getStringList(const QString &type=QString());
    private:
        OracleLoaderDriverFieldPropertiesWidget *outer;
    };

signals:
    void ddlChanged();

private slots:
    void enclosedIsOptionalToggled(bool checked);
    void populateFieldListFromColumnsTab();

private:
    void createFieldsTable(IQueryScheduler *queryScheduler);
    void createColumnTransformsTable(IStringListRetriever *columnListRetriever);

    ComboBoxAndLineEditWidget *terminatorSelector;
    LineEditAndCheckBoxWidget *enclosedByEditor;
    QComboBox *trimTypeComboBox;
    QCheckBox *missingFieldValuesNullCheckBox;
    QCheckBox *rejectNullRowsCheckBox;

    QTabWidget *fieldsTab;
    DataTableAndToolBarWidget *fieldList;
    DataTableAndToolBarWidget *columnTransformsTable;

    QStringList directoryList;

    virtual QStringList getStringList(const QString &) {
        return directoryList;
    }

    FieldNameRetriever fieldNameRetriever;
};

#endif // ORACLELOADERDRIVERFIELDPROPERTIESWIDGET_H
