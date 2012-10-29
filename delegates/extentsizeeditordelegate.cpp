#include "extentsizeeditordelegate.h"
#include "widgets/extentsizeeditorwidget.h"
#include "util/dbutil.h"

ExtentSizeEditorDelegate::ExtentSizeEditorDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *ExtentSizeEditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    ExtentSizeEditorWidget *editor=new ExtentSizeEditorWidget(true, true, parent);
    editor->setDelegateMode();

    connect(editor, SIGNAL(changed()), this, SLOT(editorDataChanged()));

    return editor;
}

void ExtentSizeEditorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    ExtentSizeEditorWidget *extentSizeEditor=static_cast<ExtentSizeEditorWidget*>(editor);
    QString data=index.data().toString().trimmed();

    //parse data
    if(data.isEmpty()){
        extentSizeEditor->clear();
        extentSizeEditor->setUnit(OraExp::GB);
    }else if(data.compare(extentSizeEditor->unlimitedText(), Qt::CaseInsensitive)==0){
        extentSizeEditor->setUnlimited(true);
    }else{
        extentSizeEditor->parseText(data);
    }

}

void ExtentSizeEditorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    ExtentSizeEditorWidget *extentSizeEditor=static_cast<ExtentSizeEditorWidget*>(editor);
    model->setData(index, extentSizeEditor->getExtentSizeClause(), Qt::EditRole);
}

void ExtentSizeEditorDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

void ExtentSizeEditorDelegate::editorDataChanged()
{
    ExtentSizeEditorWidget *editor=qobject_cast<ExtentSizeEditorWidget*>(sender());
    Q_ASSERT(editor);
    if(editor->text().trimmed().size()>0){
        emit commitData(editor);
    }
}
