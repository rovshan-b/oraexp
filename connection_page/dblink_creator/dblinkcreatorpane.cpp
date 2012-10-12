#include "dblinkcreatorpane.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/nameeditor.h"
#include "widgets/passwordeditor.h"
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
}

QLayout *DbLinkCreatorPane::createForm()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QGroupBox *generalInfoBox=new QGroupBox(tr("General"));
    QFormLayout *form1=new QFormLayout();

    dblinkNameEditor=new NameEditor();
    form1->addRow(tr("Name"), dblinkNameEditor);

    publicCheckBox=new QCheckBox();
    publicCheckBox->setChecked(!objectCreator->propertyValue("public").isEmpty());
    form1->addRow(tr("Public"), publicCheckBox);

    targetDbComboBox=new QComboBox();
    targetDbComboBox->setEditable(true);
    form1->addRow(tr("Target database"), targetDbComboBox);

    generalInfoBox->setLayout(form1);
    mainLayout->addWidget(generalInfoBox);

    QGroupBox *authenticationBox=new QGroupBox(tr("Authentication"));
    QFormLayout *form2=new QFormLayout();

    currentUserCheckBox=new QCheckBox();
    form2->addRow(tr("Use session user"), currentUserCheckBox);

    usernameEditor=new QLineEdit();
    form2->addRow(tr("Username"), usernameEditor);

    passwordEditor=new PasswordEditor();
    form2->addRow(tr("Password"), passwordEditor);

    authenticationBox->setLayout(form2);
    mainLayout->addWidget(authenticationBox);

    QGroupBox *sharedBox=new QGroupBox(tr("Sharing"));
    QFormLayout *form3=new QFormLayout();

    sharedCheckBox=new QCheckBox();
    form3->addRow(tr("Shared"), sharedCheckBox);

    sharedUsernameEditor=new QLineEdit();
    form3->addRow(tr("Authenticate as "), sharedUsernameEditor);

    sharedPasswordEditor=new PasswordEditor();
    form3->addRow(tr("Password"), sharedPasswordEditor);

    sharedBox->setLayout(form3);
    mainLayout->addWidget(sharedBox);

    enableControls();
    connect(currentUserCheckBox, SIGNAL(stateChanged(int)), this, SLOT(enableControls()));
    connect(sharedCheckBox, SIGNAL(stateChanged(int)), this, SLOT(enableControls()));

    //connect(schemaList, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(dblinkNameEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(publicCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(ddlChanged()));
    //connect(targetDbComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(targetDbComboBox->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
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

QString DbLinkCreatorPane::generateCreateDdl()
{
    return getDbLinkInfo().generateDdl();
}

QList<QueryListItem> DbLinkCreatorPane::generateAlterDdl()
{
    QList<QueryListItem> result;

    DbLinkInfo *originalDbLinkInfo=getOriginalObjectInfo<DbLinkInfo>();
    result.append(QueryListItem(this, getDbLinkInfo().generateDiffDdl(*originalDbLinkInfo, queryScheduler->getDb()->supportsAlteringDatabaseLinks())));

    return result;
}

bool DbLinkCreatorPane::beforeCreate() const
{
    DbLinkInfo info=getDbLinkInfo();
    return WidgetHelper::validate(&info, this->window());
}

bool DbLinkCreatorPane::beforeAlter() const
{
    Q_ASSERT(editMode);

    DbLinkInfo info=getDbLinkInfo();
    if(!WidgetHelper::validate(&info, this->window())){
        return false;
    }

    DbLinkInfo *originalDbLinkInfo=getOriginalObjectInfo<DbLinkInfo>();
    if(info.needsRecreation(*originalDbLinkInfo, queryScheduler->getDb()->supportsAlteringDatabaseLinks())){
        return QMessageBox::question(this->window(), tr("Drop and recreate"),
                                     tr("Database link needs to be dropped and recreated\n"
                                        "because you changed a property other than password or\n"
                                        "current database version does not support ALTER DATABASE LINK clause.\nDo you want to proceed?"),
                                     QMessageBox::Ok | QMessageBox::Cancel)==QMessageBox::Ok;
    }

    return true;
}

void DbLinkCreatorPane::alterQuerySucceeded(const QString &taskName)
{
    DbLinkInfo *originalDbLinkInfo=getOriginalObjectInfo<DbLinkInfo>();
    Q_ASSERT(originalDbLinkInfo);

    if(taskName=="drop_db_link"){
        originalDbLinkInfo->dropped=true;
    }else if(taskName=="create_db_link"){
        *originalDbLinkInfo=getDbLinkInfo();
    }else if(taskName=="rename_db_link"){
        originalDbLinkInfo->name=dblinkNameEditor->text().trimmed().toUpper();
    }else if(taskName=="alter_db_link"){
        *originalDbLinkInfo=getDbLinkInfo();
    }
}

void DbLinkCreatorPane::enableControls()
{
    usernameEditor->setEnabled(!currentUserCheckBox->isChecked());
    passwordEditor->setEnabled(!currentUserCheckBox->isChecked());

    sharedUsernameEditor->setEnabled(sharedCheckBox->isChecked());
    sharedPasswordEditor->setEnabled(sharedCheckBox->isChecked());
}

DbLinkInfo DbLinkCreatorPane::getDbLinkInfo() const
{
    DbLinkInfo info;

    info.owner=publicCheckBox->isChecked() ? "PUBLIC" : objectCreator->getOriginalSchemaName();
    info.name=dblinkNameEditor->text().trimmed().toUpper();
    info.host=targetDbComboBox->lineEdit()->text().trimmed();
    info.currentUser=currentUserCheckBox->isChecked();
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
    currentUserCheckBox->setChecked(info->currentUser);
    usernameEditor->setText(info->username);
    passwordEditor->lineEdit()->setText(info->password);
    sharedCheckBox->setChecked(info->shared);
    sharedUsernameEditor->setText(info->sharedAuthenticatedBy);
    sharedPasswordEditor->lineEdit()->setText(info->sharedIdentifiedBy);

    if(!info->isCompleteInfo){
        QMessageBox::information(this->window(), tr("Incomplete metadata loaded"),
                                 tr("Please, note that loaded information is not complete, \n"
                                    "because you do not have SELECT permission on SYS.LINK$ table.\n"
                                    "You might need to re-enter most of the information to alter database link."));
    }
}
