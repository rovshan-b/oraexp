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

    virtual bool isModified(int childIndex = 0) const;
    virtual void setModified(bool modified, int childIndex = 0);

    virtual QString getCurrentFileName(int childIndex = 0) const;
    virtual void setCurrentFileName(const QString &fileName, int childIndex = 0);

protected:
    virtual void saveToStream(QTextStream &out, int childIndex = 0);

private slots:
    void autotraceTriggered(bool checked);
    void modificationChanged(bool changed);

private:
    WorksheetWidget *worksheetWidget;

    bool shouldCheckAutotraceAction() const;

    QString currentFileName;

};

#endif // WORKSHEET_H
