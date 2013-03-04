#ifndef DATAEXPORTOPTIONSWIDGET_H
#define DATAEXPORTOPTIONSWIDGET_H

#include <QWidget>

class QStackedWidget;
class QComboBox;
class QCheckBox;
class QBoxLayout;
class LineEditWithButton;
class QTabWidget;
class DataTableAndToolBarWidget;
class DataExporterBase;


class DataExportOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataExportOptionsWidget(QWidget *parent = 0);

    void setSelectedRange(int startRow, int startColumn, int endRow, int endColumn);

    DataExporterBase *createExporter() const;

    bool validate();

private slots:
    void selectSaveFilename();
    void enableControls();
    
private:
    enum ExportFormat{
        CSV
    };

    QTabWidget *tab;

    QComboBox *formatComboBox;
    LineEditWithButton *filenameEditor;
    QComboBox *encodingComboBox;
    QCheckBox *bomCheckbox;

    QCheckBox *selectedOnlyCheckbox;
    QCheckBox *includeNullTextCheckbox;
    QCheckBox *includeColumnHeadersCheckbox;
    QCheckBox *quoteColumnHeadersCheckbox;

    QComboBox *stringQuotingComboBox;
    QComboBox *numberQuotingComboBox;

    QComboBox *lineEndingsComboBox;
    QComboBox *delimiterComboBox;
    QCheckBox *delimiterAfterLastColumnCheckbox;

    DataTableAndToolBarWidget *dataTableAndToolbar;

    //QStackedWidget *formatWidgetsTab;

    int selectionStartRow;
    int selectionStartColumn;
    int selectionEndRow;
    int selectionEndColumn;

    void createOptionsTab();
    void createDataReplacementTab();
    void createGeneralOptionsPane(QBoxLayout *layout);
    void createQuotingOptionsPane(QBoxLayout *layout);
    void createDelimiterOptionsPane(QBoxLayout *layout);

    void populateExportFormats();
    void populateQuotingOptions(QComboBox *comboBox);
    void populateLineEndingOptions(QComboBox *comboBox);
    void populateDelimiterOptions(QComboBox *comboBox);
    void populateNewlineReplacements(QComboBox *comboBox);
};

#endif // DATAEXPORTOPTIONSWIDGET_H
