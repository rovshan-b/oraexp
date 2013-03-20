#ifndef SELECTLINKEDOBJECTSDIALOG_H
#define SELECTLINKEDOBJECTSDIALOG_H

#include <QDialog>
#include "navtree/dbtreemodel.h"

class QComboBox;
class QLineEdit;
class QSpinBox;
class QCheckBox;
class QListView;
class QStandardItemModel;

class SelectLinkedObjectsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectLinkedObjectsDialog(QWidget *parent);

    QList<DbTreeModel::DbTreeNodeType> getSelectedItemTypes() const;
    void setAvailableObjectTypes(QList<DbTreeModel::DbTreeNodeType> objectTypes);

    QComboBox *relationTypeComboBox;
    QLineEdit *nameLikeEditor;
    QLineEdit *nameNotLikeEditor;
    QSpinBox *maxLevelEditor;
    //QCheckBox *strictMatchCheckBox;

public slots:
    virtual void accept();

private:
    QListView *itemTypesList;
    QStandardItemModel *itemTypesModel;

};

#endif // SELECTLINKEDOBJECTSDIALOG_H
