#include "optionsdialogsnippetstab.h"
#include "util/settings.h"
#include "widgets/datatableandtoolbarwidget.h"
#include "models/genericeditabletablemodel.h"
#include "delegates/plaintexteditordelegate.h"
#include "delegates/autoappenddelegate.h"
#include "codeeditor/codeeditor.h"
#include <QtGui>

OptionsDialogSnippetsTab::OptionsDialogSnippetsTab()
{
}

void OptionsDialogSnippetsTab::saveSettings()
{
    QAbstractItemModel *model = table->table()->model();

    QStringHash hash;
    for(int i=0; i<model->rowCount(); ++i){
        QString key = model->index(i, 0).data().toString().trimmed();
        QString value = model->index(i, 1).data().toString();

        if(!key.isEmpty() && !value.isEmpty()){
            hash[key] = value;
        }
    }

    if(!hash.isEmpty()){
        CodeEditor::textShortcuts = hash;
        Settings::saveStringHash("CodeEditor/shortcuts", hash);
    }
}

void OptionsDialogSnippetsTab::doCreateUi()
{
    QVBoxLayout *topLayout = new QVBoxLayout();

    GenericEditableTableModel *model = new GenericEditableTableModel(QStringList() << tr("Key") << tr("Value"), this);
    table = new DataTableAndToolBarWidget(model, Qt::Horizontal);
    table->table()->horizontalHeader()->resizeSection(0, 100);
    table->table()->horizontalHeader()->setStretchLastSection(true);
    table->table()->setEditTriggers(QAbstractItemView::AllEditTriggers);

    AutoAppendDelegate *keyDelegate = new AutoAppendDelegate(this);
    table->table()->setItemDelegateForColumn(0, keyDelegate);

    PlainTextEditorDelegate *valueDelegate = new PlainTextEditorDelegate(tr("Edit value"), this);
    valueDelegate->setTrim(false);
    table->table()->setItemDelegateForColumn(1, valueDelegate);

    loadSnippets(model);

    topLayout->addWidget(table);

    addToMainLayout(topLayout);
}

void OptionsDialogSnippetsTab::loadSnippets(GenericEditableTableModel *model)
{
    CodeEditor::loadTextShortcuts();

    model->ensureRowCount(CodeEditor::textShortcuts.count() + 1);

    QStringList keys = CodeEditor::textShortcuts.keys();
    keys.sort();

    for(int i=0; i<keys.size(); ++i){
        model->setData(model->index(i,0), keys.at(i));
        model->setData(model->index(i,1), CodeEditor::textShortcuts[keys.at(i)]);
    }
}
