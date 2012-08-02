#ifndef TEMPLATEDPARAMSDELEGATE_H
#define TEMPLATEDPARAMSDELEGATE_H

#include "abstracttemplatedparamsdelegate.h"

template<typename ParamClass, typename ParamDialog>
class TemplatedParamsDelegate : public AbstractTemplatedParamsDelegate<ParamClass,ParamDialog>
{
public:
    explicit TemplatedParamsDelegate(IQueryScheduler *queryScheduler, QObject *parent) : AbstractTemplatedParamsDelegate<ParamClass,ParamDialog>(queryScheduler, parent)
    {
    }

protected:

    virtual ParamDialog *createEditorDialog(bool isEditMode, QWidget *parentWindow)
    {
        ParamDialog *dialog=new ParamDialog(this->queryScheduler, parentWindow, isEditMode);
        return dialog;
    }

    virtual QString generateDdl(const ParamClass &params)
    {
        return params.generateDdl();
    }

    virtual QString generateDiffDdl(const ParamClass &params, const ParamClass &originalParams)
    {
        return params.generateDiffDdl(originalParams);
    }
};

#endif // TEMPLATEDPARAMSDELEGATE_H
