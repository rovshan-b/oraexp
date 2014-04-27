#ifndef OPTIONSDIALOGEDITORTAB_H
#define OPTIONSDIALOGEDITORTAB_H

#include "optionsdialogtab.h"

class QComboBox;

class OptionsDialogEditorTab : public OptionsDialogTab
{
public:
    OptionsDialogEditorTab();

    virtual void saveSettings();

protected:
    virtual void doCreateUi();

private:
    QComboBox *keywordCaseFoldingCombo;
    QComboBox *identifierCaseFoldingCombo;

    void fillCaseFoldingOptions(QComboBox *comboBox);
    void loadSettings();
};

#endif // OPTIONSDIALOGEDITORTAB_H
