#ifndef ABSTRACTTEMPLATEDPARAMSDELEGATE_H
#define ABSTRACTTEMPLATEDPARAMSDELEGATE_H

#include "lineeditwithbuttondelegate.h"
#include "dialogs/storageparamsdialog.h"
#include "widgets/lineeditwithbutton.h"
#include "widgets/lineeditwithautocompleter.h"
#include "beans/storageparams.h"
#include "models/genericeditabletablemodel.h"
#include <QtGui>

class IQueryScheduler;

template<typename ParamClass, typename ParamDialog>
class AbstractTemplatedParamsDelegate : public LineEditWithButtonDelegate
{
public:
    explicit AbstractTemplatedParamsDelegate(IQueryScheduler *queryScheduler, QObject *parent) : LineEditWithButtonDelegate(false, parent),
        queryScheduler(queryScheduler), frozenRowIx(-1), currentRowIx(-1), tableModel(0)
    {
    }

    virtual QWidget *createEditor(QWidget *parent,
                                  const QStyleOptionViewItem & option,
                                  const QModelIndex &index) const
    {
        QWidget *editor=LineEditWithButtonDelegate::createEditor(parent, option, index);
        LineEditWithButton *lineEditWithButton=static_cast<LineEditWithButton*>(editor);
        lineEditWithButton->lineEdit()->setReadOnly(true);

        QVariant params=index.data(Qt::UserRole);
        if(params.isValid() && params.canConvert<ParamClass>()){
            lineEditWithButton->setProperty("additional_params", params);
        }

        QVariant originalParams=index.data(Qt::UserRole+1);
        if(originalParams.isValid() && originalParams.canConvert<ParamClass>()){
            lineEditWithButton->setProperty("original_additional_params", originalParams);
        }

        Q_ASSERT(index.model()->inherits("GenericEditableTableModel"));
        tableModel=(GenericEditableTableModel*)(index.model());
        frozenRowIx=tableModel->getFrozenRow();

        currentRowIx=index.row();

        return editor;
    }


    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        LineEditWithButtonDelegate::setModelData(editor, model, index);

        LineEditWithButton *lineEditWithButton = static_cast<LineEditWithButton*>(editor);

        QVariant params=lineEditWithButton->property("additional_params");
        if(params.isValid() && params.canConvert<ParamClass>()){
            model->setData(index,
                           params,
                           Qt::UserRole);
        }
    }

protected slots:
    void buttonClicked(LineEditWithButton *senderWidget)
    {
        bool isEditMode=(currentRowIx<=frozenRowIx);

        ParamDialog *dlg=createEditorDialog(isEditMode, senderWidget->window());
        QScopedPointer<ParamDialog> dialog(dlg);

        QVariant originalParams=senderWidget->property("original_additional_params");
        bool originalParamsValid=(originalParams.isValid() && originalParams.canConvert<ParamClass>());

        QVariant params=senderWidget->property("additional_params");
        ParamClass param;
        if(params.isValid() && params.canConvert<ParamClass>()){
            param=params.value<ParamClass>();
            dialog->setParams(param, originalParamsValid ? originalParams.value<ParamClass>() : ParamClass());
        }

        if(dialog->exec()){
            ParamClass newParams=dialog->getParams();
            senderWidget->setProperty("additional_params", QVariant::fromValue(newParams));
            senderWidget->lineEdit()->setText(originalParamsValid ?
                                                  generateDiffDdl(newParams, originalParams.value<ParamClass>()) :
                                                  generateDdl(newParams));
            senderWidget->lineEdit()->setCursorPosition(0);

            emit commitData(senderWidget);
        }
    }

protected:
    IQueryScheduler *queryScheduler;
    mutable int frozenRowIx;
    mutable int currentRowIx;
    mutable GenericEditableTableModel *tableModel;

    virtual ParamDialog *createEditorDialog(bool isEditMode, QWidget *parentWindow)=0;
    virtual QString generateDdl(const ParamClass &params)=0;
    virtual QString generateDiffDdl(const ParamClass &params, const ParamClass &originalParams)=0;
};

#endif // ABSTRACTTEMPLATEDPARAMSDELEGATE_H
