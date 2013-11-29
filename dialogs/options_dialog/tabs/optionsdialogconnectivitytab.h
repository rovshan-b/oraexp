#ifndef OPTIONSDIALOGCONNECTIVITYTAB_H
#define OPTIONSDIALOGCONNECTIVITYTAB_H

#include "optionsdialogtab.h"

class QCheckBox;

class OptionsDialogConnectivityTab : public OptionsDialogTab
{
    Q_OBJECT
public:
    explicit OptionsDialogConnectivityTab(QWidget *parent = 0);

    virtual void saveSettings();

protected:
    virtual void doCreateUi();
    
private:
    QCheckBox *useSeparateSessionsCheckBox;
    
};

#endif // OPTIONSDIALOGCONNECTIVITYTAB_H
