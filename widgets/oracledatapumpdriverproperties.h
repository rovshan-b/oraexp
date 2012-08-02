#ifndef ORACLEDATAPUMPDRIVERPROPERTIES_H
#define ORACLEDATAPUMPDRIVERPROPERTIES_H

#include <QWidget>

class QComboBox;
class ComboBoxAndLineEditWidget;

class OracleDataPumpDriverProperties : public QWidget
{
    Q_OBJECT
public:
    explicit OracleDataPumpDriverProperties(QWidget *parent = 0);

    QString generateDdl() const;

signals:
    void ddlChanged();

private:
    QComboBox *encryptionComboBox;
    ComboBoxAndLineEditWidget *logFileEditor;
    QComboBox *compressionComboBox;
    ComboBoxAndLineEditWidget *versionEditor;


};

#endif // ORACLEDATAPUMPDRIVERPROPERTIES_H
