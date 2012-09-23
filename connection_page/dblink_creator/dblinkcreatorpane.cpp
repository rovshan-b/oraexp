#include "dblinkcreatorpane.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/nameeditor.h"
#include "widgets/lineeditandcheckboxwidget.h"
#include "util/strutil.h"
#include "beans/dblinkinfo.h"
#include "interfaces/iqueryscheduler.h"
#include "connectivity/dbconnection.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"
#include "util/widgethelper.h"
#include <QtGui>

DbLinkCreatorPane::DbLinkCreatorPane(DbObjectCreator *objectCreator, QWidget *parent) :
    DbObjectCreatorSimplePane(objectCreator, parent)
{

}

void DbLinkCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectCreatorSimplePane::setQueryScheduler(queryScheduler);

    /*if(queryScheduler->getDb()->getSchemaName()!=schemaList->currentText()){
        dblinkNameEditor->setEnabled(false);
    }*/
}

QLayout *DbLinkCreatorPane::createForm()
{
    //QRegExp digitsRegExp("\\d*", Qt::CaseSensitive, QRegExp::RegExp2);
    //QRegExpValidator *intValidator=new QRegExpValidator(digitsRegExp, this);

    QVBoxLayout *mainLayout=new QVBoxLayout();

    QGroupBox *generalInfoBox=new QGroupBox(tr("General"));
    QFormLayout *form1=new QFormLayout();

    //schemaList=new DbItemListComboBox(objectCreator->getOriginalSchemaName(), "user", true);
    //form1->addRow(tr("Schema"), schemaList);

    dblinkNameEditor=new NameEditor();
    form1->addRow(tr("Name"), dblinkNameEditor);

    publicCheckBox=new QCheckBox();
    form1->addRow(tr("Public"), publicCheckBox);

    targetDbComboBox=new QComboBox();
    targetDbComboBox->setEditable(true);
    form1->addRow(tr("Target database"), targetDbComboBox);

    generalInfoBox->setLayout(form1);
    mainLayout->addWidget(generalInfoBox);

    QGroupBox *authenticationBox=new QGroupBox(tr("Authentication"));
    QFormLayout *form2=new QFormLayout();

    currentUserCheckBox=new QCheckBox();
    form2->addRow(tr("Use current user"), currentUserCheckBox);

    usernameEditor=new QLineEdit();
    form2->addRow(tr("Username"), usernameEditor);

    passwordEditor=new LineEditAndCheckBoxWidget(tr("Show"));
    passwordEditor->lineEdit()->setEchoMode(QLineEdit::Password);
    form2->addRow(tr("Password"), passwordEditor);

    authenticationBox->setLayout(form2);
    mainLayout->addWidget(authenticationBox);

    QGroupBox *sharedBox=new QGroupBox(tr("Sharing"));
    QFormLayout *form3=new QFormLayout();

    sharedCheckBox=new QCheckBox();
    form3->addRow(tr("Shared"), sharedCheckBox);

    sharedUsernameEditor=new QLineEdit();
    form3->addRow(tr("Authenticated by"), sharedUsernameEditor);

    sharedPasswordEditor=new LineEditAndCheckBoxWidget(tr("Show"));
    sharedPasswordEditor->lineEdit()->setEchoMode(QLineEdit::Password);
    form3->addRow(tr("Identified by"), sharedPasswordEditor);

    sharedBox->setLayout(form3);
    mainLayout->addWidget(sharedBox);

    enableControls();
    connect(currentUserCheckBox, SIGNAL(stateChanged(int)), this, SLOT(enableControls()));
    connect(sharedCheckBox, SIGNAL(stateChanged(int)), this, SLOT(enableControls()));
    connect(passwordEditor->checkBox(), SIGNAL(stateChanged(int)), this, SLOT(showPassword()));
    connect(sharedPasswordEditor->checkBox(), SIGNAL(stateChanged(int)), this, SLOT(showPassword()));

    //connect(schemaList, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(dblinkNameEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(publicCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(ddlChanged()));
    connect(targetDbComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(targetDbComboBox, SIGNAL(editTextChanged(QString)), this, SIGNAL(ddlChanged()));
    connect(currentUserCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(ddlChanged()));
    connect(usernameEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(passwordEditor->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(sharedCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(ddlChanged()));
    connect(sharedUsernameEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(sharedPasswordEditor->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));

    return mainLayout;
}

QString DbLinkCreatorPane::getSchemaName() const
{
    return publicCheckBox->isChecked() ? "PUBLIC" : objectCreator->getOriginalSchemaName();
}

QString DbLinkCreatorPane::getObjectName() const
{
    return dblinkNameEditor->text().trimmed().toUpper();
}

bool DbLinkCreatorPane::beforeAlter() const
{
    /*
    Q_ASSERT(editMode);

    DbLinkInfo *originalDbLinkInfo=getOriginalObjectInfo<DbLinkInfo>();
    if(getDbLinkInfo().needsRecreation(*originalDbLinkInfo)){
        return QMessageBox::question(this->window(), tr("Drop and recreate"),
                                     tr("DbLink needs to be dropped and recreated in order to set new current value.\nDo you want to proceed?"),
                                     QMessageBox::Ok | QMessageBox::Cancel)==QMessageBox::Ok;
    }

    return true;*/

    return true;
}

QString DbLinkCreatorPane::generateCreateDdl()
{
    //return getDbLinkInfo().generateDdl(DbLinkCreateDdlOptions());
    return "";
}

QList<QueryListItem> DbLinkCreatorPane::generateAlterDdl()
{
    /*
    QList<QueryListItem> result;

    DbLinkInfo *originalDbLinkInfo=getOriginalObjectInfo<DbLinkInfo>();
    result.append(QueryListItem(this, getDbLinkInfo().generateDiffDdl(*originalDbLinkInfo, DbLinkDiffDdlOptions())));

    return result;*/

    return QList<QueryListItem>();
}

void DbLinkCreatorPane::alterQuerySucceeded(const QString &taskName)
{
    /*
    DbLinkInfo *originalDbLinkInfo=getOriginalObjectInfo<DbLinkInfo>();
    Q_ASSERT(originalDbLinkInfo);

    if(taskName=="drop_dblink"){
        originalDbLinkInfo->dropped=true;
    }else if(taskName=="create_dblink"){
        *originalDbLinkInfo=getDbLinkInfo();
    }else if(taskName=="rename_dblink"){
        originalDbLinkInfo->name=dblinkNameEditor->text().trimmed().toUpper();
    }else if(taskName=="alter_dblink"){
        *originalDbLinkInfo=getDbLinkInfo();
    }*/
}

void DbLinkCreatorPane::enableControls()
{
    usernameEditor->setEnabled(!currentUserCheckBox->isChecked());
    passwordEditor->setEnabled(!currentUserCheckBox->isChecked());

    sharedUsernameEditor->setEnabled(sharedCheckBox->isChecked());
    sharedPasswordEditor->setEnabled(sharedCheckBox->isChecked());
}

void DbLinkCreatorPane::showPassword()
{
    passwordEditor->lineEdit()->setEchoMode(passwordEditor->checkBox()->isChecked() ? QLineEdit::Normal : QLineEdit::Password);
    sharedPasswordEditor->lineEdit()->setEchoMode(sharedPasswordEditor->checkBox()->isChecked() ? QLineEdit::Normal : QLineEdit::Password);
}

DbLinkInfo DbLinkCreatorPane::getDbLinkInfo() const
{
    DbLinkInfo info;

    info.owner=publicCheckBox->isChecked() ? "PUBLIC" : objectCreator->getOriginalSchemaName();
    info.name=dblinkNameEditor->text().trimmed().toUpper();
    info.host=targetDbComboBox->lineEdit()->text().trimmed();
    info.username=usernameEditor->text().trimmed().toUpper();
    info.password=passwordEditor->lineEdit()->text().trimmed();
    info.shared=sharedCheckBox->isChecked();
    info.sharedAuthenticatedBy=sharedUsernameEditor->text().trimmed().toUpper();
    info.sharedIdentifiedBy=sharedPasswordEditor->lineEdit()->text().trimmed();

    return info;
}

void DbLinkCreatorPane::setObjectInfo(DbObjectInfo *objectInfo)
{
    DbLinkInfo *info=static_cast<DbLinkInfo*>(objectInfo);
    Q_ASSERT(info);

    dblinkNameEditor->setText(info->name);
    publicCheckBox->setChecked(info->owner=="PUBLIC");
    WidgetHelper::setComboBoxText(targetDbComboBox, info->host);
    usernameEditor->setText(info->username);
    passwordEditor->lineEdit()->setText(info->password);
    sharedCheckBox->setChecked(info->shared);
    sharedUsernameEditor->setText(info->sharedAuthenticatedBy);
    sharedPasswordEditor->lineEdit()->setText(info->sharedIdentifiedBy);
}
