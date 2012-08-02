#include "oracleloaderdriverrecordpropertieswidget.h"
#include "widgets/comboboxandlineeditwidget.h"
#include <QtGui>

OracleLoaderDriverRecordPropertiesWidget::OracleLoaderDriverRecordPropertiesWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout=new QHBoxLayout();

    QFormLayout *form1=new QFormLayout();

    QIntValidator *intValidator=new QIntValidator(this);

    recordDelimiterTypeComboBox=new QComboBox();
    recordDelimiterTypeComboBox->addItem("");
    recordDelimiterTypeComboBox->addItem(tr("Delimiter"));
    recordDelimiterTypeComboBox->addItem(tr("Length"));
    recordDelimiterTypeComboBox->setCurrentIndex(0);
    form1->addRow(tr("Separate records by"), recordDelimiterTypeComboBox);

    delimiterSelector=new ComboBoxAndLineEditWidget(false);
    delimiterSelector->comboBox()->addItem(tr("Newline"));
    delimiterSelector->comboBox()->addItem(tr("String"));
    delimiterSelector->comboBox()->addItem(tr("Hexadecimal"));
    delimiterSelector->comboBox()->setCurrentIndex(0);
    delimiterSelector->comboBox()->setMinimumWidth(150);
    form1->addRow(tr("Delimiter"), delimiterSelector);

    recordSizeEditor=new ComboBoxAndLineEditWidget(true, tr("Record size"));
    recordSizeEditor->comboBox()->addItem(tr("Fixed"));
    recordSizeEditor->comboBox()->addItem(tr("Variable"));
    recordSizeEditor->comboBox()->setMinimumWidth(150);
    form1->addRow(tr("Record size"), recordSizeEditor);

    charactersetEditor=new QLineEdit();
    form1->addRow(tr("Character set"), charactersetEditor);

    languageEditor=new QLineEdit();
    form1->addRow(tr("Language"), languageEditor);

    territoryEditor=new QLineEdit();
    form1->addRow(tr("Territory"), territoryEditor);

    endianComboBox=new QComboBox();
    endianComboBox->addItem("");
    endianComboBox->addItem(tr("Big endian"));
    endianComboBox->addItem(tr("Little endian"));
    form1->addRow(tr("Byte order"), endianComboBox);

    checkByteOrderMaskComboBox=new QComboBox();
    checkByteOrderMaskComboBox->addItem("");
    checkByteOrderMaskComboBox->addItem(tr("Enable"));
    checkByteOrderMaskComboBox->addItem(tr("Disable"));
    form1->addRow(tr("Check byte order mark"), checkByteOrderMaskComboBox);

    stringSizesComboBox=new QComboBox();
    stringSizesComboBox->addItem("");
    stringSizesComboBox->addItem(tr("Bytes"));
    stringSizesComboBox->addItem(tr("Characters"));
    form1->addRow(tr("String sizes in"), stringSizesComboBox);


    layout->addLayout(form1);

    QFormLayout *form2=new QFormLayout();

    loadWhenCondition=new QLineEdit();
    form2->addRow(tr("Load WHEN"), loadWhenCondition);

    badFileEditor=new ComboBoxAndLineEditWidget(false, tr("Bad file name"));
    badFileEditor->comboBox()->addItem("");
    badFileEditor->comboBox()->addItem(tr("Enable"));
    badFileEditor->comboBox()->addItem(tr("Disable"));
    form2->addRow(tr("Bad file"), badFileEditor);

    discardFileEditor=new ComboBoxAndLineEditWidget(false, tr("Discard file name"));
    discardFileEditor->comboBox()->addItem("");
    discardFileEditor->comboBox()->addItem(tr("Enable"));
    discardFileEditor->comboBox()->addItem(tr("Disable"));
    form2->addRow(tr("Discard file"), discardFileEditor);

    logFileEditor=new ComboBoxAndLineEditWidget(false, tr("Log file name"));
    logFileEditor->comboBox()->addItem("");
    logFileEditor->comboBox()->addItem(tr("Enable"));
    logFileEditor->comboBox()->addItem(tr("Disable"));
    form2->addRow(tr("Log file"), logFileEditor);

    readSizeEditor=new QLineEdit();
    readSizeEditor->setValidator(intValidator);
    form2->addRow(tr("Read size"), readSizeEditor);

    dateCacheSizeEditor=new QLineEdit();
    dateCacheSizeEditor->setValidator(intValidator);
    form2->addRow(tr("Date cache size"), dateCacheSizeEditor);

    skipCountEditor=new QLineEdit();
    skipCountEditor->setValidator(intValidator);
    form2->addRow(tr("Skip n records"), skipCountEditor);

    disableDirectoryLinkCheckBox=new QCheckBox();
    form2->addRow(tr("Disable directory link check"), disableDirectoryLinkCheckBox);

    layout->addLayout(form2);

    setLayout(layout);


    enableDelimiterTypeSelector(0);
    enableDelimiterLineEditor(0);

    connect(recordDelimiterTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableDelimiterTypeSelector(int)));
    connect(delimiterSelector->comboBox(), SIGNAL(currentIndexChanged(int)), this, SLOT(enableDelimiterLineEditor(int)));

    connect(recordDelimiterTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(delimiterSelector, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
    connect(recordSizeEditor, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
    connect(charactersetEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(languageEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(territoryEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(endianComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(checkByteOrderMaskComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(stringSizesComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(loadWhenCondition, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(badFileEditor, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
    connect(discardFileEditor, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
    connect(logFileEditor, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
    connect(readSizeEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(dateCacheSizeEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(skipCountEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(disableDirectoryLinkCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(ddlChanged()));
}

void OracleLoaderDriverRecordPropertiesWidget::enableDelimiterTypeSelector(int selectedDelimiterTypeIndex)
{
    delimiterSelector->setEnabled(selectedDelimiterTypeIndex==1);
    recordSizeEditor->setEnabled(selectedDelimiterTypeIndex==2);
}

void OracleLoaderDriverRecordPropertiesWidget::enableDelimiterLineEditor(int selectedDelimiterTypeIndex)
{
    delimiterSelector->lineEdit()->setEnabled(selectedDelimiterTypeIndex!=0);
}

void OracleLoaderDriverRecordPropertiesWidget::parallelEditorChanged(ComboBoxAndLineEditWidget *parallelEditor)
{
    skipCountEditor->setEnabled(parallelEditor->comboBox()->currentIndex()!=1);
    if(skipCountEditor->isEnabled()){
        skipCountEditor->setToolTip("");
    }else{
        skipCountEditor->setToolTip(tr("Disabled because you specified parallel option on General tab"));
    }
}

QString OracleLoaderDriverRecordPropertiesWidget::generateDdl() const
{
    QString ddl;

    int delimType=recordDelimiterTypeComboBox->currentIndex();

    if(delimType!=0){
        ddl.append("\n\tDELIMITED BY");
    }

    if(delimType==1){
        int recordDelimType=delimiterSelector->comboBox()->currentIndex();
        if(recordDelimType==0){
            ddl.append(" NEWLINE");
        }else if(recordDelimType==1){
            ddl.append(" '").append(delimiterSelector->lineEdit()->text()).append("'");
        }else if(recordDelimType==2){
            ddl.append(" 0X'").append(delimiterSelector->lineEdit()->text()).append("'");
        }
    }else if(delimType==2){
        int recordSizeType=recordSizeEditor->comboBox()->currentIndex();
        if(recordSizeType==0){
            ddl.append(" FIXED");
        }else if(recordSizeType==1){
            ddl.append(" VARIABLE");
        }
        ddl.append(" ").append(recordSizeEditor->lineEdit()->text());
    }

    QString characterSet=charactersetEditor->text().trimmed();
    if(!characterSet.isEmpty()){
        ddl.append("\n\tCHARACTERSET '").append(characterSet).append("'");
    }

    QString language=languageEditor->text().trimmed();
    if(!language.isEmpty()){
        ddl.append("\n\tLANGUAGE '").append(language).append("'");
    }

    QString territory=territoryEditor->text().trimmed();
    if(!territory.isEmpty()){
        ddl.append("\n\tTERRITORY '").append(territory).append("'");
    }

    int byteOrder=endianComboBox->currentIndex();
    if(byteOrder==1){
        ddl.append("\n\tDATA IS BIG ENDIAN");
    }else if(byteOrder==2){
        ddl.append("\n\tDATA IS LITTLE ENDIAN");
    }

    int checkByteOrder=checkByteOrderMaskComboBox->currentIndex();
    if(checkByteOrder==1){
        ddl.append("\n\tBYTEORDERMARK CHECK");
    }else if(checkByteOrder==2){
        ddl.append("\n\tBYTEORDERMARK NOCHECK");
    }

    int stringSizes=stringSizesComboBox->currentIndex();
    if(stringSizes==1){
        ddl.append("\n\tSTRING SIZES ARE IN BYTES");
    }else if(stringSizes==2){
        ddl.append("\n\tSTRING SIZES ARE IN CHARACTERS");
    }

    QString loadWhen=loadWhenCondition->text().trimmed();
    if(!loadWhen.isEmpty()){
        ddl.append("\n\tLOAD WHEN (").append(loadWhen).append(")");
    }

    int badFile=badFileEditor->comboBox()->currentIndex();
    if(badFile==1){
        ddl.append("\n\tBADFILE ").append("'").append(badFileEditor->lineEdit()->text()).append("'");
    }else if(badFile==2){
        ddl.append("\n\tNOBADFILE");
    }

    int discardFile=discardFileEditor->comboBox()->currentIndex();
    if(discardFile==1){
        ddl.append("\n\tDISCARDFILE ").append("'").append(discardFileEditor->lineEdit()->text()).append("'");
    }else if(discardFile==2){
        ddl.append("\n\tNODISCARDFILE");
    }

    int logFile=logFileEditor->comboBox()->currentIndex();
    if(logFile==1){
        ddl.append("\n\tLOGFILE ").append("'").append(logFileEditor->lineEdit()->text()).append("'");
    }else if(logFile==2){
        ddl.append("\n\tNOLOGFILE");
    }

    QString readSize=readSizeEditor->text();
    if(!readSize.isEmpty()){
        ddl.append("\n\tREADSIZE ").append(readSize);
    }

    if(disableDirectoryLinkCheckBox->isChecked()){
        ddl.append("\n\tDISABLE_DIRECTORY_LINK_CHECK");
    }

    QString dateCacheSize=dateCacheSizeEditor->text();
    if(!dateCacheSize.isEmpty()){
        ddl.append("\n\tDATE CACHE ").append(dateCacheSize);
    }

    QString skipCount=skipCountEditor->text().trimmed();
    if(!skipCount.isEmpty() && skipCountEditor->isEnabled()){
        ddl.append("\n\tSKIP ").append(skipCount);
    }

    if(ddl.length()>0){
        ddl.prepend("\n\tRECORDS");
    }

    return ddl;
}
