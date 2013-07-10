#ifndef SESSIONBROWSERTABS_H
#define SESSIONBROWSERTABS_H

#include "widgets/genericqueryviewertabs.h"

class Param;

class SessionBrowserTabs : public GenericQueryViewerTabs
{
    Q_OBJECT
public:
    explicit SessionBrowserTabs(DbUiManager *uiManager,
                                QWidget *parent = 0);

    virtual void createTabs();

    QList<Param*> getQueryParams();

    void setInstId(int instId) {this->instId = instId;}
    void setSid(int sid) {this->sid = sid;}
    void setSerial(int serial) {this->serial = serial;}
    void refresh();

public slots:
    void setCurrentSession(int instId, int sid, int serial);
    
private:
    int instId;
    int sid;
    int serial;
    
};

#endif // SESSIONBROWSERTABS_H
