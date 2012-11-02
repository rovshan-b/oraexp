#include "booleandelegate.h"
#include "widgets/centeredcheckboxwidget.h"
#include "util/iconutil.h"
#include <QtGui>

BooleanDelegate::BooleanDelegate(QObject *parent, bool defaultValue) :
    QStyledItemDelegate(parent), defaultValue(defaultValue)
{
    QCheckBox checkbox;
    checkbox.setStyleSheet("background-color:transparent");
    uncheckedPixmap=QPixmap::grabWidget(&checkbox);
    checkbox.setChecked(true);
    checkedPixmap=QPixmap::grabWidget(&checkbox);
}

void BooleanDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    QVariant value=index.data();
    if (!value.isValid() || qVariantCanConvert<bool>(value)) {
        bool boolVal = value.isValid() ? value.toBool() : defaultValue;

        QStyle *style=qApp->style();

        QRect checkBoxRect=style->subElementRect(QStyle::SE_CheckBoxIndicator, &option);
        int chkWidth=checkBoxRect.width();
        int chkHeight=checkBoxRect.height();

        int centerX=option.rect.left() + qMax(option.rect.width()/2-chkWidth/2, 0);
        int centerY=option.rect.top() + qMax(option.rect.height()/2-chkHeight/2, 0);
        QStyleOptionViewItem modifiedOption(option);
        modifiedOption.rect.moveTo(centerX, centerY);
        modifiedOption.rect.setSize(QSize(chkWidth, chkHeight));

        if((option.state & QStyle::State_Enabled) != QStyle::State_Enabled){
            painter->fillRect(option.rect, qApp->palette().background());
        }

        painter->drawPixmap(modifiedOption.rect, boolVal ? checkedPixmap : uncheckedPixmap);

        /*QStyle *style=qApp->style();

        QRect checkBoxRect=style->subElementRect(QStyle::SE_CheckBoxIndicator, &option);
        int chkWidth=checkBoxRect.width();
        int chkHeight=checkBoxRect.height();

        int centerX=option.rect.left() + qMax(option.rect.width()/2-chkWidth/2, 0);
        int centerY=option.rect.top() + qMax(option.rect.height()/2-chkHeight/2, 0);
        QStyleOptionViewItem modifiedOption(option);
        modifiedOption.rect.moveTo(centerX, centerY);
        modifiedOption.rect.setSize(QSize(chkWidth, chkHeight));

        if(boolVal){
            modifiedOption.state |= QStyle::State_On;
            modifiedOption.state &= ~QStyle::State_Off;
        }else{
            modifiedOption.state &= ~QStyle::State_On;
            modifiedOption.state |= QStyle::State_Off;
        }

        if((option.state & QStyle::State_Enabled) != QStyle::State_Enabled){
            painter->fillRect(option.rect, qApp->palette().background());
        }

        style->drawPrimitive(QStyle::PE_IndicatorCheckBox, &modifiedOption, painter);
        */
    }else{
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QWidget *BooleanDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
{
    CenteredCheckBoxWidget *editor=new CenteredCheckBoxWidget(parent);
    editor->checkBox()->setChecked(defaultValue);
    connect(editor->checkBox(), SIGNAL(toggled(bool)), this, SLOT(checkStateChanged()));

    return editor;
}

void BooleanDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QVariant data=index.model()->data(index, Qt::EditRole);
    bool value = data.isNull() ? defaultValue : data.toBool();

    CenteredCheckBoxWidget *checkBoxWidget = static_cast<CenteredCheckBoxWidget*>(editor);
    checkBoxWidget->checkBox()->setChecked(value);
}

void BooleanDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    CenteredCheckBoxWidget *checkBoxWidget = static_cast<CenteredCheckBoxWidget*>(editor);

    bool value = checkBoxWidget->checkBox()->isChecked();

    model->setData(index, value, Qt::EditRole);
}

void BooleanDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    CenteredCheckBoxWidget *checkBoxWidget = static_cast<CenteredCheckBoxWidget*>(editor);

    QSize size=checkBoxWidget->sizeHint();
    editor->setMinimumHeight(size.height());
    editor->setMinimumWidth(size.width());
    editor->setGeometry(option.rect);
}

void BooleanDelegate::checkStateChanged()
{
    QWidget *checkBox=qobject_cast<QWidget*>(sender());
    Q_ASSERT(checkBox);
    emit commitData(checkBox->parentWidget());
}
