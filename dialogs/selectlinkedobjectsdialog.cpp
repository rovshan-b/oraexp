#include "selectlinkedobjectsdialog.h"
#include "util/dialoghelper.h"
#include "util/dbutil.h"
#include "util/iconutil.h"
#include <QtGui>

SelectLinkedObjectsDialog::SelectLinkedObjectsDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Find linked objects"));

    QVBoxLayout *mainLayout=new QVBoxLayout();

    itemTypesList = new QListView();
    itemTypesModel = new QStandardItemModel(0, 1, this);
    itemTypesList->setModel(itemTypesModel);
    mainLayout->addWidget(itemTypesList);

    QFormLayout *form=new QFormLayout();

    relationTypeComboBox = new QComboBox();
    relationTypeComboBox->addItem(tr("Dependent objects"));
    relationTypeComboBox->addItem(tr("Referenced objects"));
    relationTypeComboBox->addItem(tr("Dependent and Referenced objects"));
    relationTypeComboBox->setCurrentIndex(2);
    form->addRow(tr("Find"), relationTypeComboBox);

    maxLevelEditor = new QSpinBox();
    maxLevelEditor->setRange(0, 10000);
    maxLevelEditor->setToolTip(tr("Maximum number of steps to go. Zero means unlimited."));
    form->addRow(tr("Max level"), maxLevelEditor);

    nameLikeEditor = new QLineEdit();
    nameLikeEditor->setText("%");
    nameLikeEditor->setMaxLength(50);
    nameLikeEditor->setToolTip(tr("Pattern object names must match."));
    form->addRow(tr("Name like"), nameLikeEditor);

    nameNotLikeEditor = new QLineEdit();
    nameNotLikeEditor->setMaxLength(50);
    nameNotLikeEditor->setToolTip(tr("Pattern object names must not match."));
    form->addRow(tr("Name not like"), nameNotLikeEditor);

    /*
    strictMatchCheckBox = new QCheckBox();
    strictMatchCheckBox->setToolTip(tr("Do not scan through other object types"));
    form->addRow(tr("Strict match"), strictMatchCheckBox);*/

    mainLayout->addLayout(form);

    QDialogButtonBox *buttonBox=DialogHelper::createButtonBox(this);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

QList<DbTreeModel::DbTreeNodeType> SelectLinkedObjectsDialog::getSelectedItemTypes() const
{
    QList<DbTreeModel::DbTreeNodeType> result;

    for(int i=0; i<itemTypesModel->rowCount(); ++i){
        QStandardItem *item = itemTypesModel->item(i);
        if(item->isCheckable() && item->checkState()==Qt::Checked){
            result.append((DbTreeModel::DbTreeNodeType)item->data().toInt());
        }
    }

    if(itemTypesModel->rowCount()==1 && result.isEmpty()){
        result.append((DbTreeModel::DbTreeNodeType)itemTypesModel->item(0)->data().toInt());
    }

    return result;
}

void SelectLinkedObjectsDialog::setAvailableObjectTypes(QList<DbTreeModel::DbTreeNodeType> objectTypes)
{
    Q_ASSERT(objectTypes.size()>0);

    itemTypesModel->clear();

    for(int i=0; i<objectTypes.size(); ++i){
        DbTreeModel::DbTreeNodeType nodeType = objectTypes.at(i);
        QStandardItem *item = new QStandardItem(
                    IconUtil::getIcon(DbUtil::getDbObjectIconNameByParentNodeType(nodeType)),
                    DbUtil::getDbObjectDisplayNameByNodeType(nodeType)
                    );
        item->setData(nodeType);
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        itemTypesModel->appendRow(item);
    }

    bool singleEntry = (objectTypes.size()==1);
    itemTypesList->setVisible(!singleEntry);
    //strictMatchCheckBox->setVisible(!singleEntry);
}

void SelectLinkedObjectsDialog::accept()
{
    if(getSelectedItemTypes().isEmpty()){
        QMessageBox::critical(this, tr("Nothing selected"),
                              tr("Please, select at least one item type from list"));
    }else{
        QDialog::accept();
    }
}
