#include "savechangesdialog.h"
#include "connection_page/connectionpage.h"
#include "connection_page/connectionpagetab.h"
#include "models/checkablestandarditemmodel.h"
#include "util/dialoghelper.h"
#include <QtGui>

SaveChangesDialog::SaveChangesDialog(bool showDiscardButton, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Save changes"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    tree = new QTreeView();
    model = new CheckableStandardItemModel(0, 1, this);
    tree->setModel(model);
    tree->setHeaderHidden(true);
    mainLayout->addWidget(tree);

    QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::Save | QDialogButtonBox::Cancel;

    if(showDiscardButton){
        buttons |= QDialogButtonBox::Discard;
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox();
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(buttons);

    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);


    connect(buttonBox, SIGNAL(accepted()), this, SLOT(save()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    if(showDiscardButton){
        connect(buttonBox->button(QDialogButtonBox::Discard), SIGNAL(clicked()), this, SLOT(discard()));
    }

    resize(400, 300);
}

void SaveChangesDialog::addConnection(ConnectionPage *cnPage, const QIcon &icon, const QString &title)
{
    if(currentCnPage==cnPage){
        return;
    }

    currentCnPage = cnPage;

    currentParentItem = new QStandardItem(icon, title);
    currentParentItem->setCheckable(true);
    currentParentItem->setCheckState(Qt::Checked);
    currentParentItem->setData(QVariant::fromValue((void*)cnPage));
    model->appendRow(currentParentItem);
}

void SaveChangesDialog::addTab(ConnectionPageTab *tab, const QIcon &icon, const QString &title)
{
    if(tab->isModified()){
        addRow(tab, icon, title);
    }
}

bool SaveChangesDialog::isEmpty() const
{
    return model->rowCount() == 0;
}

void SaveChangesDialog::setReady()
{
    tree->expandAll();
}

void SaveChangesDialog::save()
{
    QStandardItem *rootItem = model->invisibleRootItem();
    for(int i=0; i<rootItem->rowCount(); ++i){
        QStandardItem *connectionItem = rootItem->child(i);
        for(int k=0; k<connectionItem->rowCount(); ++k){
            QStandardItem *tabItem = connectionItem->child(k);
            ConnectionPageTab *tab = (ConnectionPageTab*)tabItem->data().value<void*>();

            if(!tab->saveAll()){
                return;
            }
        }
    }

    accept();
}

void SaveChangesDialog::discard()
{
    accept();
}

void SaveChangesDialog::addRow(ConnectionPageTab *tab, const QIcon &icon, const QString &title)
{
    QStandardItem *tabItem = new QStandardItem(icon, title);
    tabItem->setCheckable(true);
    tabItem->setCheckState(Qt::Checked);
    tabItem->setData(QVariant::fromValue((void*)tab));
    currentParentItem->appendRow(tabItem);
}
