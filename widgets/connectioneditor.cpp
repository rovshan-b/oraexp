#include "connectioneditor.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/dbutil.h"
#include "enums.h"
#include "beans/dbconnectioninfo.h"
#include "beans/environmentinfo.h"
#include <QtGui>

ConnectionEditor::ConnectionEditor(QWidget *parent) :
    QWidget(parent),
    currentConnection(0)
{
    createUi();
}

void ConnectionEditor::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QFormLayout *topForm = new QFormLayout();

    titleEditor = new QLineEdit();
    topForm->addRow(tr("Connection name"), titleEditor);

    environmentCombo = new QComboBox();
    QList<EnvironmentInfo> environments = DbUtil::getEnvironmentList();
    foreach(const EnvironmentInfo &env, environments){
        environmentCombo->addItem(env.icon, env.title, env.environment);
    }
    topForm->addRow(tr("Environment"), environmentCombo);

    usernameEditor = new QLineEdit();
    topForm->addRow(tr("Username"), usernameEditor);

    passwordEditor = new QLineEdit();
    passwordEditor->setEchoMode(QLineEdit::Password);
    topForm->addRow(tr("Password"), passwordEditor);

    savePasswordCheckBox = new QCheckBox();
    savePasswordCheckBox->setChecked(true);
    topForm->addRow(tr("Save password"), savePasswordCheckBox);

    connectionTypeCombo = new QComboBox();
    connectionTypeCombo->addItem(tr("Direct"), Direct);
    connectionTypeCombo->addItem(tr("TNS"), TNS);
    topForm->addRow(tr("Connection type"), connectionTypeCombo);

    connectionDetailsTab = new QStackedWidget();
    connectionDetailsTab->setFrameShadow(QFrame::Raised);
    connectionDetailsTab->setFrameShape(QFrame::StyledPanel);
    connectionDetailsTab->addWidget(createDirectPane());
    connectionDetailsTab->addWidget(createTnsPane());
    topForm->addRow(connectionDetailsTab);

    connectAsCombo = new QComboBox();
    connectAsCombo->addItem(tr("Normal"), OraExp::ConnectAsNormal);
    connectAsCombo->addItem(tr("SYSDBA"), OraExp::ConnectAsSysdba);
    connectAsCombo->addItem(tr("SYSOPER"), OraExp::ConnectAsSysoper);
    topForm->addRow(tr("Connect as"), connectAsCombo);

    mainLayout->addLayout(topForm);

    mainLayout->addStretch();

    setLayout(mainLayout);

    connect(connectionTypeCombo, SIGNAL(currentIndexChanged(int)), connectionDetailsTab, SLOT(setCurrentIndex(int)));
}

QWidget *ConnectionEditor::createTnsPane()
{
    QWidget *tnsPane = new QWidget();
    QFormLayout *form = new QFormLayout();

    tnsCombo = new QComboBox();
    tnsCombo->setEditable(true);
    form->addRow(tr("TNS"), tnsCombo);

    loadTnsList();

    //form->setContentsMargins(0,0,0,0);
    tnsPane->setLayout(form);
    return tnsPane;
}

QWidget *ConnectionEditor::createDirectPane()
{
    QWidget *directPane = new QWidget();
    QFormLayout *form = new QFormLayout();

    hostEditor = new QLineEdit();
    hostEditor->setText("localhost");
    form->addRow(tr("Host"), hostEditor);

    portEditor = new QLineEdit();
    portEditor->setValidator(new QIntValidator(1, 65535, this));
    portEditor->setText("1521");
    form->addRow(tr("Port"), portEditor);

    sidEditor = new QLineEdit();
    sidEditor->setText("XE");
    form->addRow(tr("SID"), sidEditor);

    //form->setContentsMargins(0,0,0,0);
    directPane->setLayout(form);
    return directPane;
}

void ConnectionEditor::loadTnsList()
{
    QByteArray tnsnamesPathArr = qgetenv("TNS_ADMIN");
    bool appendNetworkDir = false;
    if(tnsnamesPathArr.isEmpty()){
        tnsnamesPathArr = qgetenv("ORACLE_HOME");
        appendNetworkDir = true;
    }

    QString path(tnsnamesPathArr);
    if(!path.endsWith('\\') && !path.endsWith('/')){
        path.append('/');
    }
    if(appendNetworkDir){
        path.append("network/admin/");
    }
    path.append("tnsnames.ora");

    if(!QFile::exists(path)){
        return;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream strm(&file);
        QString contents=strm.readAll();
        QRegExp tnsnameRegExp("([^\\s]+)\\s*=\\s*\\s\\(DESCRIPTION\\s*=", Qt::CaseInsensitive, QRegExp::RegExp2);

        int pos = 0;

        while ((pos = tnsnameRegExp.indexIn(contents, pos)) != -1) {
            tnsCombo->addItem(tnsnameRegExp.cap(1));

            pos += tnsnameRegExp.matchedLength();
        }

        if(tnsCombo->count()>0){
            tnsCombo->model()->sort(0);
            tnsCombo->setCurrentIndex(0);
        }
    }
}

DbConnectionInfo *ConnectionEditor::createConnection()
{
    Q_ASSERT(currentConnection == 0);

    if(!validate()){
        return 0;
    }

    DbConnectionInfo *connection = new DbConnectionInfo();
    connection->uuid = QUuid::createUuid().toString();
    updateConnection(connection);

    this->currentConnection = connection;

    emit hasConnection(true);

    return connection;
}

void ConnectionEditor::updateConnection(DbConnectionInfo *connection)
{
    QString title = titleEditor->text().trimmed();
    OraExp::ConnectionEnvironment environment = (OraExp::ConnectionEnvironment) environmentCombo->itemData(environmentCombo->currentIndex()).toInt();
    QString username = usernameEditor->text().trimmed().toUpper();
    QString password = passwordEditor->text();
    bool savePassword = savePasswordCheckBox->isChecked();
    int type = connectionTypeCombo->currentIndex();

    QString connectionString;

    if(type == 0){
        QString hostname = hostEditor->text().trimmed();
        QString port = portEditor->text().trimmed();
        QString sid = sidEditor->text().trimmed();

        connectionString = QString("%1:%2/%3").arg(hostname, port, sid);
    }else{
        connectionString = tnsCombo->currentText();
    }

    OraExp::ConnectAs connectAs = (OraExp::ConnectAs) connectAsCombo->itemData(connectAsCombo->currentIndex()).toInt();

    connection->title = title;
    connection->environment = environment;
    connection->username = username;
    connection->password = password;
    connection->savePassword = savePassword;
    connection->type = type;
    connection->connectionString = connectionString;
    connection->connectAs = connectAs;
}

void ConnectionEditor::setCurrentConnection(DbConnectionInfo *connectionInfo)
{
    currentConnection = connectionInfo;

    titleEditor->setText(currentConnection ? currentConnection->title : "");
    environmentCombo->setCurrentIndex(currentConnection ? environmentCombo->findData(connectionInfo->environment) : 0);
    usernameEditor->setText(currentConnection ? currentConnection->username : "");
    passwordEditor->setText(currentConnection ? currentConnection->password : "");
    savePasswordCheckBox->setChecked(currentConnection ? currentConnection->savePassword : true);

    if(currentConnection){
        connectionTypeCombo->setCurrentIndex(currentConnection->type);
        if(currentConnection->type == 0){
            QStringList parts = currentConnection->connectionString.split('/');
            if(parts.size() == 2){
                sidEditor->setText(parts.at(1));

                parts = parts.at(0).split(':');
                hostEditor->setText(parts.at(0));
                portEditor->setText(parts.size()>1 ? parts.at(1) : "1521");
            }
        }else{
            WidgetHelper::setComboBoxText(tnsCombo, currentConnection->connectionString);
        }
    }else{
        hostEditor->setText("localhost");
        portEditor->setText("1521");
        sidEditor->setText("XE");
        WidgetHelper::setComboBoxText(tnsCombo, "");
    }

    connectAsCombo->setCurrentIndex(currentConnection ? connectAsCombo->findData(currentConnection->connectAs) : 0);

    emit hasConnection(connectionInfo != 0);
}

DbConnectionInfo *ConnectionEditor::getCurrentConnection() const
{
    return currentConnection;
}

void ConnectionEditor::updateCurrentConnection()
{
    Q_ASSERT(currentConnection);

    updateConnection(currentConnection);
}

void ConnectionEditor::deleteCurrentConnection()
{
    delete currentConnection;
    currentConnection = 0;
}

bool ConnectionEditor::validate(bool showErrors)
{
    QStringList messages;

    QString title = titleEditor->text().trimmed();

    if(title.isEmpty()){
         messages.append(tr(" - Connection name not entered"));
    }

    int type = connectionTypeCombo->currentIndex();

    if(type == 0){
        QString hostname = hostEditor->text().trimmed();
        QString port = portEditor->text().trimmed();
        QString sid = sidEditor->text().trimmed();

        if(hostname.isEmpty()){
            messages.append(tr(" - Host name not entered"));
        }

        if(port.isEmpty()){
            messages.append(tr(" - Port not entered"));
        }

        if(sid.isEmpty()){
            messages.append(tr(" - SID not entered"));
        }
    }else{
        QString tns = tnsCombo->currentText();

        if(tns.isEmpty()){
            messages.append(tr(" - TNS not entered"));
        }
    }

    if(showErrors && messages.size() > 0){
        QMessageBox::critical(this->window(),
                              tr("Missing input"),
                              tr("Please, correct following errors and try again:\n%1").arg(messages.join("\n")));
    }

    return messages.size() == 0;
}

void ConnectionEditor::focus()
{
    titleEditor->setFocus();
}
