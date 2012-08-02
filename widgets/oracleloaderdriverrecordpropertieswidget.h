#ifndef ORACLELOADERDRIVERRECORDPROPERTIES_H
#define ORACLELOADERDRIVERRECORDPROPERTIES_H

#include <QWidget>

class QComboBox;
class ComboBoxAndLineEditWidget;
class QLineEdit;
class QCheckBox;

class OracleLoaderDriverRecordPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OracleLoaderDriverRecordPropertiesWidget(QWidget *parent = 0);

    QString generateDdl() const;

signals:
    void ddlChanged();

public slots:
    void parallelEditorChanged(ComboBoxAndLineEditWidget *parallelEditor);

private slots:
    void enableDelimiterTypeSelector(int selectedDelimiterTypeIndex);
    void enableDelimiterLineEditor(int selectedDelimiterTypeIndex);

private:
    QComboBox *recordDelimiterTypeComboBox;
    ComboBoxAndLineEditWidget *delimiterSelector;
    ComboBoxAndLineEditWidget *recordSizeEditor;

    QLineEdit *charactersetEditor;
    QLineEdit *languageEditor;
    QLineEdit *territoryEditor;
    QComboBox *endianComboBox;
    QComboBox *checkByteOrderMaskComboBox;
    QComboBox *stringSizesComboBox;
    QLineEdit *loadWhenCondition;

    ComboBoxAndLineEditWidget *badFileEditor;
    ComboBoxAndLineEditWidget *discardFileEditor;
    ComboBoxAndLineEditWidget *logFileEditor;

    QLineEdit *readSizeEditor;
    QCheckBox *disableDirectoryLinkCheckBox;
    QLineEdit *dateCacheSizeEditor;
    QLineEdit *skipCountEditor;

};

#endif // ORACLELOADERDRIVERRECORDPROPERTIES_H
