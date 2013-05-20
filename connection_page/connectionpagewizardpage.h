#ifndef CONNECTIONPAGEWIZARDPAGE_H
#define CONNECTIONPAGEWIZARDPAGE_H

#include <QWizardPage>

class ConnectionPageWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ConnectionPageWizardPage(QWidget *parent = 0);
    
    virtual bool isBusy() const {return false;}

signals:
    
public slots:
    
};

#endif // CONNECTIONPAGEWIZARDPAGE_H
