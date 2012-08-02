#include "checkboxlistdelegate.h"

#include <QtGui>

CheckBoxListDelegate::CheckBoxListDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void CheckBoxListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    bool isChecked=index.data(Qt::UserRole).toBool();
    QString itemText=index.data().toString();

    const QStyle *style=QApplication::style();
    QStyleOptionButton opt;
    opt.state |= isChecked ? QStyle::State_On : QStyle::State_Off;
    opt.state |= QStyle::State_Enabled;
    opt.text=itemText;
    opt.rect=option.rect;

    style->drawControl(QStyle::CE_CheckBox, &opt, painter);
}

QWidget *CheckBoxListDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
{
    QCheckBox *editor=new QCheckBox(parent);
    return editor;
}

void CheckBoxListDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QCheckBox *checkBox=(QCheckBox*)editor;
    checkBox->setText(index.data().toString());
    checkBox->setChecked(index.data(Qt::UserRole).toBool());
}

void CheckBoxListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QCheckBox *checkBox=(QCheckBox*)editor;
    bool isChecked=checkBox->isChecked();

    QMap<int, QVariant> data;
    data.insert(Qt::DisplayRole, checkBox->text());
    data.insert(Qt::UserRole, isChecked);
    model->setItemData(index, data);
}

void CheckBoxListDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
     editor->setGeometry(option.rect);
}

