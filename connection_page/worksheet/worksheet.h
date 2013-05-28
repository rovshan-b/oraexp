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
    QString getContents() const;

    void insertText(const QString &text);

    virtual bool canSave() const {return true;}
    virtual bool canPrint() const {return true;}

    virtual void focusAvailable();

    void setAutotraceEnabled(bool enabled);
    bool isAutotraceEnabled() const;

    virtual bool isModified() const;
    virtual void setModified(bool modified);

    virtual QString getCurrentFileName() const;
    virtual void setCurrentFileName(const QString &fileName);

protected:
    virtual void saveToStream(QTextStream &out);

private slots:
    void autotraceTriggered(bool checked);
    void modificationChanged(bool changed);

private:
    WorksheetWidget *worksheetWidget;

    bool shouldCheckAutotraceAction() const;

    QString currentFileName;

};

#endif // WORKSHEET_H
