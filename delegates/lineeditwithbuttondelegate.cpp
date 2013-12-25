#include "lineeditwithbuttondelegate.h"
#include "widgets/lineeditwithbutton.h"
#include "widgets/lineeditwithautocompleter.h"
#include "dialogs/columnselectordialog.h"
#include <QtGui>
#include <QtDebug>

LineEditWithButtonDelegate::LineEditWithButtonDelegate(bool convertToUpperCase, QObject *parent) :
    QStyledItemDelegate(parent), convertToUpperCase(convertToUpperCase), trim(true), autoAppendRows(true)
{
}

/*void LineEditWithButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    if (qVariantCanConvert<QString>(index.data())) {
        QString value = index.data().toString();

        if (option.state & QStyle::State_Selected){
            painter->setPen(option.palette.highlightedText().color());
        }

        int newlineIndex=value.indexOf('\n');
        if(newlineIndex!=-1){
            value.truncate(newlineIndex);
            value.append(" ...");
        }

        QTextOption textOptions(Qt::AlignVCenter);
        textOptions.setWrapMode(QTextOption::NoWrap);

        QVariant backgoundRole=index.data(Qt::BackgroundRole);
        if(backgoundRole.isValid()){
            QBrush backgoundBrush=backgoundRole.value<QBrush>();
            painter->fillRect(option.rect, backgoundBrush);
        }

        QStylePainter stylePainter(painter->);
        stylePainter.drawItemText(option.rect, textOptions.flags(), option.palette, index.flags()==Qt::NoItemFlags, value);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}*/

QWidget *LineEditWithButtonDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
{
    LineEditWithButton *editor=new LineEditWithButton(parent);
    editor->setAutoFillBackground(true);
    editor->lineEdit()->setFrame(false);
    editor->lineEdit()->setToolTip(this->tooltipText);

    connect(editor, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(buttonClicked(LineEditWithButton *)));   
    connect(editor->lineEdit(), SIGNAL(editingFinished()), this, SLOT(editingFinished()));

    return editor;
}

void LineEditWithButtonDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    LineEditWithButton *lineEditWithButton=static_cast<LineEditWithButton*>(editor);
    lineEditWithButton->lineEdit()->setText(index.data().toString());
}

void LineEditWithButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    LineEditWithButton *lineEditWithButton = static_cast<LineEditWithButton*>(editor);
    QString dataToSet=lineEditWithButton->lineEdit()->text();
    if(trim){
        dataToSet = dataToSet.trimmed();
    }
    if(convertToUpperCase){
        dataToSet=dataToSet.toUpper();
    }
    model->setData(index, dataToSet, Qt::EditRole);

    if(autoAppendRows && !dataToSet.isEmpty()){
        int modelRowCount=model->rowCount();
        if(modelRowCount-1==index.row()){
            model->insertRows(modelRowCount, 1);
        }
    }
}

void LineEditWithButtonDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    //LineEditWithButton *lineEditWithButton=static_cast<LineEditWithButton*>(editor);
    //lineEditWithButton->browseButton()->setMaximumHeight(lineEditWithButton->lineEdit()->sizeHint().height());
    editor->setGeometry(option.rect);
}

void LineEditWithButtonDelegate::buttonClicked(LineEditWithButton *)
{

}

void LineEditWithButtonDelegate::setTooltip(const QString &tooltip){
    this->tooltipText=tooltip;
}

void LineEditWithButtonDelegate::setAutoConvertToUpperCase(bool convert)
{
    this->convertToUpperCase=convert;
}

void LineEditWithButtonDelegate::setAutoAppendRows(bool autoAppendRows)
{
    this->autoAppendRows = autoAppendRows;
}

void LineEditWithButtonDelegate::setTrim(bool trim)
{
    this->trim = trim;
}

void LineEditWithButtonDelegate::editingFinished()
{
    QLineEdit *lineEdit=qobject_cast<QLineEdit*>(sender());
    if(lineEdit){
        emit commitData(lineEdit->parentWidget());
    }
}
