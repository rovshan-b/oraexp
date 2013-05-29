#include "checkedlistboxwidget.h"
#include "util/widgethelper.h"
#include <QtGui>

CheckedListBoxWidget::CheckedListBoxWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    listView = new QListView();
    model = new QStandardItemModel(0, 1, this);
    listView->setModel(model);
    mainLayout->addWidget(listView);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    selectAllButton = new QPushButton(tr("All"));
    buttonsLayout->addWidget(selectAllButton);

    selectNoneButton = new QPushButton(tr("None"));
    buttonsLayout->addWidget(selectNoneButton);

    buttonsLayout->addStretch();

    mainLayout->addLayout(buttonsLayout);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    connect(selectAllButton, SIGNAL(clicked()), this, SLOT(selectAll()));
    connect(selectNoneButton, SIGNAL(clicked()), this, SLOT(selectNone()));
}

void CheckedListBoxWidget::addItem(const QPixmap &pixmap, const QString &text, bool checked)
{
    QStandardItem *item = new QStandardItem(pixmap, text);
    item->setCheckable(true);
    item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    model->appendRow(item);
}

QStringList CheckedListBoxWidget::getCheckedItemNames() const
{
    QStringList result;

    for(int i=0; i<model->rowCount(); ++i){
        QStandardItem *item = model->item(i);
        if(item->checkState() == Qt::Checked){
            result.append(item->text());
        }
    }

    return result;
}

void CheckedListBoxWidget::selectAll()
{
    WidgetHelper::checkModelItems(model, true);
}

void CheckedListBoxWidget::selectNone()
{
    WidgetHelper::checkModelItems(model, false);
}
