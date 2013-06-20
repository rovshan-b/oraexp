#ifndef PAIRCODECREATOR_H
#define PAIRCODECREATOR_H

#include "connection_page/connectionpagetab.h"
#include "navtree/dbtreemodel.h"
#include "codecreatorwidget.h"

class QStackedWidget;

class PairCodeCreator : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit PairCodeCreator(
            const QString &schemaName,
            const QString &objectName,
            DbTreeModel::DbTreeNodeType objectType,
            DbUiManager *uiManager, QWidget *parent = 0);

    virtual void createUi();
    virtual void setConnection(DbConnection *db);

    virtual bool canSave() const {return true;}

    virtual void focusAvailable();

    virtual bool isModified(int childIndex = 0) const;
    virtual void setModified(bool modified, int childIndex = 0);
    virtual QString getDefaultSaveSuffix() const;

    virtual bool isSaved() const;
    virtual QString getCurrentFileName(int childIndex = 0) const;
    virtual void setCurrentFileName(const QString &fileName, int childIndex = 0);

protected:
    virtual void saveToStream(QTextStream &out, int childIndex = 0);
    CodeCreatorWidget *childCreator(int childIndex) const;

private slots:
    void creatorInitialized();
    void switchToSpec();
    void switchToBody();
    void modificationChanged(bool changed);

    virtual bool saveAll();

private:
    CodeCreatorWidget *currentCreator() const;

    QStackedWidget *tab;

    CodeCreatorWidget *specCreatorWidget;
    CodeCreatorWidget *bodyCreatorWidget;

    int initializedCount;
    bool isSpecType;
    
};

#endif // PAIRCODECREATOR_H
