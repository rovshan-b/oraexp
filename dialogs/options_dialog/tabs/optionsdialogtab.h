#ifndef OPTIONSDIALOGTAB_H
#define OPTIONSDIALOGTAB_H

#include <QWidget>

class QBoxLayout;

class OptionsDialogTab : public QWidget
{
    Q_OBJECT
public:
    explicit OptionsDialogTab(QWidget *parent = 0);

    void createUi();

    virtual void saveSettings() = 0;

    bool isInitialized() const;
    
protected:
    virtual void doCreateUi() = 0;

    void addToMainLayout(QBoxLayout *childLayout);
    
};

#endif // OPTIONSDIALOGTAB_H
