#include "optionsdialogeditortab.h"
#include "codeeditor/codeeditor.h"
#include "util/widgethelper.h"
#include <QtGui>

OptionsDialogEditorTab::OptionsDialogEditorTab()
{
}

void OptionsDialogEditorTab::doCreateUi()
{
    QFormLayout *form = new QFormLayout();

    keywordCaseFoldingCombo = new QComboBox();
    fillCaseFoldingOptions(keywordCaseFoldingCombo);
    form->addRow(tr("Keywords"), keywordCaseFoldingCombo);

    identifierCaseFoldingCombo = new QComboBox();
    fillCaseFoldingOptions(identifierCaseFoldingCombo);
    form->addRow(tr("Identifiers"), identifierCaseFoldingCombo);

    loadSettings();

    QVBoxLayout *groupBoxContainer = new QVBoxLayout();
    groupBoxContainer->setContentsMargins(0,0,0,0);
    groupBoxContainer->addWidget(WidgetHelper::createGroupBox(form, tr("Automatic case folding")));

    addToMainLayout(groupBoxContainer);
}

void OptionsDialogEditorTab::saveSettings()
{
    CodeEditor::keywordCaseFolding = (CodeEditor::CaseFoldingType)keywordCaseFoldingCombo->currentIndex();
    CodeEditor::identifierCaseFolding = (CodeEditor::CaseFoldingType)identifierCaseFoldingCombo->currentIndex();
}

void OptionsDialogEditorTab::fillCaseFoldingOptions(QComboBox *comboBox)
{
    comboBox->addItem(tr("Unmodified"));
    comboBox->addItem(tr("Upper case"));
    comboBox->addItem(tr("Lower case"));
}

void OptionsDialogEditorTab::loadSettings()
{
    keywordCaseFoldingCombo->setCurrentIndex((int)CodeEditor::keywordCaseFolding);
    identifierCaseFoldingCombo->setCurrentIndex((int)CodeEditor::identifierCaseFolding);
}
