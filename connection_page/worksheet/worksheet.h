#ifndef WORKSHEET_H
#define WORKSHEET_H

#include <QWidget>
#include "worksheetwidget.h"

class DbUiManager;

class Worksheet : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit Worksheet(DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~Worksheet();

    virtual void setConnection(DbConnection *db);

    void setContents(const QString &contents);

    virtual bool canSave() const {return true;}
    virtual bool canPrint() const {return true;}

    virtual void focusAvailable();

    void setAutotraceEnabled(bool enabled);
    bool isAutotraceEnabled() const;

private slots:
    void autotraceTriggered(bool checked);

private:
    WorksheetWidget *worksheetWidget;

    bool shouldCheckAutotraceAction() const;

};

#endif // WORKSHEET_H
