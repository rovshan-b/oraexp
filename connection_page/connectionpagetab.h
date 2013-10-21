#ifndef CONNECTIONPAGETAB_H
#define CONNECTIONPAGETAB_H

#include <QWidget>
#include <QTextStream>
#include "connectionpageobject.h"

class QLabel;
class QTextDocument;

class ConnectionPageTab : public QWidget, public ConnectionPageObject
{
    Q_OBJECT
public:
    static const char *TAB_NAME_KEY;
    static const char *CHILD_OBJECT_NAME_KEY;


    explicit ConnectionPageTab(DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~ConnectionPageTab();

    virtual QObject *getQueryEndMonitor() {return this;}

    virtual bool canSave() const {return false;}
    virtual bool canPrint() const {return false;}

    virtual void focusAvailable() {}

    void setProperties(const QHash<QString,QString> &properties);
    void setPropertyValue(const QString &propName, const QString &propValue);
    QString propertyValue(const QString &propName) const;

    virtual void highlightChildObject(){}

    virtual QString getTitle() const;
    void setTitle(const QString &title);
    void setCaption(const QString &caption);

    virtual QString getDisplayName() const;
    virtual QIcon getIcon() const;

    QString getTabId() const;
    void setTabId(const QString &tabId);

    virtual bool isModified(int childIndex = 0) const {return false; Q_UNUSED(childIndex);}
    virtual void setModified(bool modified, int childIndex = 0) {Q_UNUSED(modified); Q_UNUSED(childIndex);}
    virtual QString getDefaultSaveSuffix() const {return "sql";}
    virtual QList<QTextDocument*> getSavePreviewDocuments() const {return QList<QTextDocument*>();}

    virtual bool isSaved() const {return !(getCurrentFileName().isEmpty());}
    virtual QString getCurrentFileName(int childIndex = 0) const {Q_UNUSED(childIndex); return "";}
    virtual void setCurrentFileName(const QString &fileName, int childIndex = 0) {Q_UNUSED(fileName); Q_UNUSED(childIndex);}
signals:
    void busyStateChanged(ConnectionPageObject *tab, bool busy);
    void stateChanged();

    void initCompleted(ConnectionPageObject *tab);
    void captionChanged(ConnectionPageTab *tab, const QString &caption);

public slots:
    virtual void queryExecTaskCompleted(const QString &taskName);
    virtual void emitBusyStateChangedSignal();
    virtual void emitInitCompletedSignal();

    bool saveContents(int childIndex = 0);
    bool saveContentsAs();
    virtual bool saveAll();

protected:
    QHash<QString,QString> properties;

    //returns all tabs of same type on same connection
    QList<ConnectionPageTab *> getPeerTabs(int limit = -1) const;

    QString tabTitile;
    QString tabId;

    virtual void saveToStream(QTextStream &out, int childIndex = 0) {Q_UNUSED(out); Q_UNUSED(childIndex);}

    void setModifiedStatusToCaption(bool changed);

private:
    bool saveToFile(const QString &fileName, int childIndex = 0);

};

#endif // CONNECTIONPAGETAB_H
