#include "bindparamsdialog.h"
#include "widgets/bindparameditorwidget.h"
#include "util/dialoghelper.h"
#include <QtGui>

BindParamsDialog::BindParamsDialog(const QStringList &bindParams, QWidget *parent) :
    QDialog(parent), bindParams(bindParams)
{
    setWindowTitle(tr("Bind parameters"));

    createUi();
}

void BindParamsDialog::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(createForm());

    mainLayout->addWidget(scrollArea);

    mainLayout->addWidget(DialogHelper::createButtonBox(this));

    setLayout(mainLayout);
}

QWidget *BindParamsDialog::createForm()
{
    QWidget *w=new QWidget();

    QFormLayout *form=new QFormLayout();
    BindParamEditorWidget *editor;

    for(int i=0; i<bindParams.size(); ++i){
        editor=new BindParamEditorWidget();
        form->addRow(bindParams.at(i), editor);
    }

    //form->setContentsMargins(0,0,0,0);
    w->setLayout(form);

    return w;
}
