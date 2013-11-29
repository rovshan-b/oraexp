#ifndef CONNECTIONPAGEWIZARDPAGE_H
#define CONNECTIONPAGEWIZARDPAGE_H

#include <QWizardPage>

class DbUiManager;

class ConnectionPageWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ConnectionPageWizardPage(DbUiManager *uiManager, QWidget *parent = 0);
    
    virtual bool isBusy() const {return false;}

protected:
    DbUiManager *uiManager;
    
};

#endif // CONNECTIONPAGEWIZARDPAGE_H
