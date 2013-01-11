#include "bindparamsdialog.h"
#include "widgets/bindparameditorwidget.h"
#include "util/dialoghelper.h"
#include "util/param.h"
#include "beans/bindparaminfo.h"
#include <QStringList>
#include <QtGui>

BindParamsDialog::BindParamsDialog(const QStringList &bindParams,
                                   const QHash<QString, BindParamInfo *> &paramHistory,
                                   QWidget *parent) :
    QDialog(parent), bindParams(bindParams), paramHistory(paramHistory), controlToSetFocus(0)
{
    setWindowTitle(tr("Bind parameters"));

    createUi();
}

QList<Param *> BindParamsDialog::getParams() const
{
    Q_ASSERT(bindParams.size()==editors.size());

    QList<Param*> params;

    for(int i=0; i<editors.size(); ++i){
        BindParamEditorWidget *editor=editors.at(i);
        QString paramName = bindParams.at(i);

        params.append(editor->createParam(paramName));
    }

    return params;
}

void BindParamsDialog::accept()
{
    QStringList messages;

    for(int i=0; i<editors.size(); ++i){
        BindParamEditorWidget *editor=editors.at(i);
        QString paramName = bindParams.at(i);

        if(editor->getBindParamType()==BindParamInfo::Date){
            QString value = editor->getBindParamValue();
            DateTime dt(value);
            if(!dt.isValid()){
                messages.append(QString("%1 entered as a value of %2 is not a valid date.").arg(value, paramName));
            }
        }
    }

    if(messages.size()==0){
        QDialog::accept();
    }else{
        QString message=messages.join("\n");
        QMessageBox::critical(this, tr("Incorrect input format"), message);
    }
}

void BindParamsDialog::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(createForm());
    scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(scrollArea);

    mainLayout->addWidget(DialogHelper::createButtonBox(this));

    setLayout(mainLayout);

    controlToSetFocus->setFocusToEditor();
}

QWidget *BindParamsDialog::createForm()
{
    QWidget *w=new QWidget();

    QFormLayout *form=new QFormLayout();
    BindParamEditorWidget *editor;
    QString paramName;
    QStringList nameParts;

    for(int i=0; i<bindParams.size(); ++i){
        paramName=bindParams.at(i);
        editor=new BindParamEditorWidget();
        editors.append(editor);

        BindParamInfo *paramInfo = paramHistory.value(paramName);

        if(paramInfo==0){
            nameParts=paramName.split('_');
            if(nameParts.contains("DATE", Qt::CaseInsensitive) ||
                    nameParts.contains("TIME", Qt::CaseInsensitive) ||
                    nameParts.contains("DATETIME", Qt::CaseInsensitive)){
                editor->setBindParamType(BindParamInfo::Date);
            }
        }else{
            editor->setBindParamInfo(paramInfo);
        }



        form->addRow(paramName, editor);

        if(i==0){
            controlToSetFocus=editor;
        }
    }

    w->setLayout(form);

    return w;
}
