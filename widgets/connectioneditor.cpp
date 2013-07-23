#include "connectioneditor.h"
#include <QtGui>

ConnectionEditor::ConnectionEditor(QWidget *parent) :
    QWidget(parent)
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
    topForm->addRow(tr("Environment"), environmentCombo);

    usernameEditor = new QLineEdit();
    topForm->addRow(tr("Username"), usernameEditor);

    passwordEditor = new QLineEdit();
    passwordEditor->setEchoMode(QLineEdit::Password);
    topForm->addRow(tr("Password"), passwordEditor);

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
