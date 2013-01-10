#include "bindparamsdialog.h"
#include "widgets/bindparameditorwidget.h"
#include "util/dialoghelper.h"
#include "util/param.h"
#include <QStringList>
#include <QtGui>

BindParamsDialog::BindParamsDialog(const QStringList &bindParams, QWidget *parent) :
    QDialog(parent), bindParams(bindParams)
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

void BindParamsDialog::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(createForm());
    scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(scrollArea);

    mainLayout->addWidget(DialogHelper::createButtonBox(this));

    setLayout(mainLayout);
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

        nameParts=paramName.split('_');
        if(nameParts.contains("DATE", Qt::CaseInsensitive) ||
                nameParts.contains("TIME", Qt::CaseInsensitive) ||
                nameParts.contains("DATETIME", Qt::CaseInsensitive)){
            editor->setBindParamType(BindParamEditorWidget::Date);
        }

        form->addRow(paramName, editor);
    }

    w->setLayout(form);

    return w;
}
