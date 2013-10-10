#include "orderbyoptionsdialog.h"
#include "util/dialoghelper.h"
#include "util/widgethelper.h"
#include "util/settings.h"
#include <QtGui>

OrderByOptionsDialog::OrderByOptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Sorting options"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    sortDirectionGroup = new QButtonGroup(this);
    nullsOrderingGroup = new QButtonGroup(this);

    QFormLayout *form = new QFormLayout();

    QRadioButton *ascButton = new QRadioButton(tr("Asc"));
    sortDirectionGroup->addButton(ascButton, 0);
    QRadioButton *descButton = new QRadioButton(tr("Desc"));
    sortDirectionGroup->addButton(descButton, 1);
    QRadioButton *removeButton = new QRadioButton(tr("Remove"));
    sortDirectionGroup->addButton(removeButton, 2);

    form->addRow(tr("Direction"), WidgetHelper::nestWidgets(QList<QWidget*>() << ascButton << descButton << removeButton, Qt::Horizontal));

    QRadioButton *nullsFirstButton = new QRadioButton(tr("First"));
    nullsOrderingGroup->addButton(nullsFirstButton, 0);
    QRadioButton *nullsLastButton = new QRadioButton(tr("Last"));
    nullsOrderingGroup->addButton(nullsLastButton, 1);
    QRadioButton *nullsDefaultButton = new QRadioButton(tr("Default"));
    nullsOrderingGroup->addButton(nullsDefaultButton, 2);

    form->addRow(tr("NULLS"), WidgetHelper::nestWidgets(QList<QWidget*>() << nullsFirstButton << nullsLastButton << nullsDefaultButton, Qt::Horizontal));

    mainLayout->addLayout(form);
    mainLayout->addWidget(DialogHelper::createButtonBox(this));

    setLayout(mainLayout);

    int lastCheckedDir = Settings::value("OrderByOptionsDialog/direction", 0).toInt();
    int lastCheckedNulls = Settings::value("OrderByOptionsDialog/nullsOrdering", 2).toInt();

    sortDirectionGroup->buttons().at(qBound(0,lastCheckedDir,sortDirectionGroup->buttons().size()))->setChecked(true);
    nullsOrderingGroup->buttons().at(qBound(0,lastCheckedNulls,nullsOrderingGroup->buttons().size()))->setChecked(true);

    connect(removeButton, SIGNAL(toggled(bool)), this, SLOT(removeSortChecked(bool)));
}

void OrderByOptionsDialog::accept()
{
    Settings::setValue("OrderByOptionsDialog/direction", sortDirectionGroup->checkedId());
    Settings::setValue("OrderByOptionsDialog/nullsOrdering", nullsOrderingGroup->checkedId());

    QDialog::accept();
}

QString OrderByOptionsDialog::getOrderByClause() const
{
    QString result;

    SortDirection dir = (SortDirection)sortDirectionGroup->checkedId();
    NullsOrdering nulls = (NullsOrdering)nullsOrderingGroup->checkedId();

    if(dir == Remove){
        return result;
    }

    result = "ORDER BY %1 ";

    switch(dir){
    case Asc:
        result.append("ASC");
        break;
    default:
        result.append("DESC");
        break;
    }

    switch(nulls){
    case First:
        result.append(" NULLS FIRST");
        break;
    case Last:
        result.append(" NULLS LAST");
        break;
    default:
        break;
    }

    return result;
}

void OrderByOptionsDialog::removeSortChecked(bool checked)
{
    foreach(QAbstractButton *btn, nullsOrderingGroup->buttons()){
        btn->setEnabled(!checked);
    }
}
