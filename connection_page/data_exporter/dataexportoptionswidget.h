#ifndef DATAEXPORTOPTIONSWIDGET_H
#define DATAEXPORTOPTIONSWIDGET_H

#include <QWidget>
#include "exporters/dataexporterbase.h"

class QStackedWidget;
class QComboBox;
class QCheckBox;
class QGroupBox;
class QLineEdit;
class QBoxLayout;
class QFormLayout;
class LineEditWithButton;
class QTabWidget;
class DataTableAndToolBarWidget;


class DataExportOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataExportOptionsWidget(bool selfContained=true, QWidget *parent = 0);

    void createUi();
    void createSeparatedUi(QFormLayout *fileControlsForm, QTabWidget *optionsTab);

    void setSelectedRange(int startRow, int startColumn, int endRow, int endColumn);
    void setTableName(const QString &schemaName, const QString &tableName);

    DataExporterBase *createExporter();

    void setMultiTableMode();

    bool validate();

signals:
    void exportFormatChanged(DataExporterBase::ExportFormat newFormat);

private slots:
    void selectSaveFilename();
    void enableControls();
    void correctFileSuffix();
    void fileFormatChanged();
    void includeSchemaCheckBoxChanged();
    
private:
    QTabWidget *tab;

    QComboBox *formatComboBox;
    LineEditWithButton *filenameEditor;
    QComboBox *encodingComboBox;
    QCheckBox *bomCheckbox;

    QCheckBox *selectedOnlyCheckbox;
    QCheckBox *includeNullTextCheckbox;
    QCheckBox *includeColumnHeadersCheckbox;
    QCheckBox *quoteColumnHeadersCheckbox;

    QGroupBox *quotingOptionsBox;
    QComboBox *stringQuotingComboBox;
    QComboBox *numberQuotingComboBox;

    QGroupBox *delimiterOptionsBox;
    QComboBox *lineEndingsComboBox;
    QComboBox *delimiterComboBox;
    QCheckBox *delimiterAfterLastColumnCheckbox;

    QGroupBox *tableNameOptionsBox;
    QCheckBox *includeSchemaCheckBox;
    QLineEdit *schemaNameEditor;
    QLineEdit *tableNameEditor;

    DataTableAndToolBarWidget *dataTableAndToolbar;

    //QStackedWidget *formatWidgetsTab;

    int selectionStartRow;
    int selectionStartColumn;
    int selectionEndRow;
    int selectionEndColumn;

    bool multiTableExport;

    void connectSignalsAndSlots();

    void createFileOptionsControls(QFormLayout *form);
    QWidget *createOptionsTab();
    QWidget *createDataReplacementTab();
    void createGeneralOptionsPane(QBoxLayout *layout);
    void createQuotingOptionsPane(QBoxLayout *layout);
    void createDelimiterOptionsPane(QBoxLayout *layout);
    void createTargetTableOptionsPane(QBoxLayout *layout);

    void populateExportFormats();
    void populateQuotingOptions(QComboBox *comboBox);
    void populateLineEndingOptions(QComboBox *comboBox);
    void populateDelimiterOptions(QComboBox *comboBox);
    void populateNewlineReplacements(QComboBox *comboBox);
};

#endif // DATAEXPORTOPTIONSWIDGET_H
