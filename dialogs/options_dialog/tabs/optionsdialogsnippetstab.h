#ifndef OPTIONSDIALOGSNIPPETSTAB_H
#define OPTIONSDIALOGSNIPPETSTAB_H

#include "optionsdialogtab.h"

class DataTableAndToolBarWidget;
class GenericEditableTableModel;

class OptionsDialogSnippetsTab : public OptionsDialogTab
{
public:
    OptionsDialogSnippetsTab();

    virtual void saveSettings();

protected:
    virtual void doCreateUi();

private:
    DataTableAndToolBarWidget *table;

    void loadSnippets(GenericEditableTableModel *model);
};

#endif // OPTIONSDIALOGSNIPPETSTAB_H
