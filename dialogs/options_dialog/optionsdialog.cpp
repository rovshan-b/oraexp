#include "optionsdialog.h"
#include "util/iconutil.h"
#include "util/dialoghelper.h"
#include "tabs/optionsdialogconnectivitytab.h"
#include "tabs/optionsdialogeditortab.h"
#include "tabs/optionsdialogsnippetstab.h"
#include <QtGui>

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Options"));
    setWindowIcon(IconUtil::getIcon("settings"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *centerLayout = new QHBoxLayout();

    createLeftPane(centerLayout);
    createRightPane(centerLayout);
    addTabs();

    mainLayout->addLayout(centerLayout);

    QDialogButtonBox *buttonBox = DialogHelper::createButtonBox(this, QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    connect(buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
    connect(sidePane, SIGNAL(currentRowChanged(int)), this, SLOT(changeTab(int)));

    sidePane->setCurrentRow(0, QItemSelectionModel::SelectCurrent);

    setStyleSheet("QToolBar { border: 0px }");

    resize(650, 400);
}

void OptionsDialog::createLeftPane(QBoxLayout *layout)
{
    QVBoxLayout *leftLayout = new QVBoxLayout();

    sidePane = new QListWidget();
    leftLayout->addWidget(sidePane);

    layout->addLayout(leftLayout);
    layout->setAlignment(leftLayout, Qt::AlignLeft | Qt::AlignTop);
}

void OptionsDialog::createRightPane(QBoxLayout *layout)
{
    QVBoxLayout *rightLayout = new QVBoxLayout();

    titleLabel = new QLabel();
    rightLayout->addWidget(titleLabel);

    tab = new QStackedWidget();

    rightLayout->addWidget(tab);

    layout->addLayout(rightLayout, 1);
    //layout->setAlignment(rightLayout, Qt::AlignLeft | Qt::AlignTop);
}

void OptionsDialog::addTabs()
{
    sidePane->addItem(new QListWidgetItem(IconUtil::getIcon("connect"), tr("Connectivity")));
    tab->addWidget(new OptionsDialogConnectivityTab());

    sidePane->addItem(new QListWidgetItem(IconUtil::getIcon("edit"), tr("Editor")));
    tab->addWidget(new OptionsDialogEditorTab());

    sidePane->addItem(new QListWidgetItem(IconUtil::getIcon("ddl"), tr("Snippets")));
    tab->addWidget(new OptionsDialogSnippetsTab());

    QString maxTitle;
    for(int i=0; i<sidePane->count(); ++i){
        QString title = sidePane->item(i)->text();
        if(title.length() > maxTitle.length()){
            maxTitle = title;
        }
    }

    sidePane->setMaximumWidth(sidePane->fontMetrics().width(maxTitle)+50);
}

void OptionsDialog::accept()
{
    apply();

    QDialog::accept();
}

void OptionsDialog::apply()
{
    for(int i=0; i<tab->count(); ++i){
        OptionsDialogTab *page = dynamic_cast<OptionsDialogTab*>(tab->widget(i));
        Q_ASSERT(page);

        if(page->isInitialized()){
            page->saveSettings();
        }
    }
}

void OptionsDialog::changeTab(int tabIx)
{
    OptionsDialogTab *page = dynamic_cast<OptionsDialogTab*>(tab->widget(tabIx));
    Q_ASSERT(page);

    titleLabel->setText(tr("<h3>%1</h3>").arg(sidePane->item(tabIx)->text()));

    if(!page->isInitialized()){
        page->createUi();
    }

    tab->setCurrentIndex(tabIx);
}
