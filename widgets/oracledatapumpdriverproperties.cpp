#include "oracledatapumpdriverproperties.h"
#include "widgets/comboboxandlineeditwidget.h"

#include <QtGui>

OracleDataPumpDriverProperties::OracleDataPumpDriverProperties(QWidget *parent) :
    QWidget(parent)
{
    QFormLayout *layout=new QFormLayout();

    encryptionComboBox=new QComboBox();
    encryptionComboBox->addItem("");
    encryptionComboBox->addItem(tr("Enable"));
    encryptionComboBox->addItem(tr("Disable"));
    layout->addRow(tr("Encryption"), encryptionComboBox);

    logFileEditor=new ComboBoxAndLineEditWidget(false, tr("[Log file name]"));
    logFileEditor->comboBox()->addItem("");
    logFileEditor->comboBox()->addItem(tr("Enable"));
    logFileEditor->comboBox()->addItem(tr("Disable"));
    layout->addRow(tr("Log file"), logFileEditor);

    compressionComboBox=new QComboBox();
    compressionComboBox->addItem("");
    compressionComboBox->addItem(tr("Enable"));
    compressionComboBox->addItem(tr("Disable"));
    layout->addRow(tr("Compression"), compressionComboBox);

    QStringList stringsToDisable;
    stringsToDisable.append(tr("Compatible"));
    stringsToDisable.append(tr("Latest"));
    versionEditor=new ComboBoxAndLineEditWidget(false, tr("[Version]"), stringsToDisable);
    versionEditor->comboBox()->addItem("");
    versionEditor->comboBox()->addItem(tr("Compatible"));
    versionEditor->comboBox()->addItem(tr("Latest"));
    versionEditor->comboBox()->addItem(tr("Specified"));
    layout->addRow(tr("Version"), versionEditor);

    setLayout(layout);

    connect(encryptionComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(logFileEditor, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
    connect(compressionComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(versionEditor, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
}

QString OracleDataPumpDriverProperties::generateDdl() const
{
    QString ddl;

    int encryption=encryptionComboBox->currentIndex();
    if(encryption==1){
        ddl.append("\n\tENCRYPTION ENABLED");
    }else if(encryption==2){
        ddl.append("\n\tENCRYPTION DISABLED");
    }

    int logFile=logFileEditor->comboBox()->currentIndex();
    if(logFile==1){
        ddl.append("\n\tLOGFILE '").append(logFileEditor->lineEdit()->text()).append("'");
    }else if(logFile==2){
        ddl.append("\n\tNOLOGFILE");
    }

    int compression=compressionComboBox->currentIndex();
    if(compression==1){
        ddl.append("\n\tCOMPRESSION ENABLED");
    }else if(compression==2){
        ddl.append("\n\tCOMPRESSION DISABLED");
    }

    int version=versionEditor->comboBox()->currentIndex();
    if(version==1){
        ddl.append("\n\tVERSION COMPATIBLE");
    }else if(version==2){
        ddl.append("\n\tVERSION LATEST");
    }else if(version==3){
        ddl.append("\n\tVERSION '").append(versionEditor->lineEdit()->text().trimmed()).append("'");
    }

    return ddl;
}
