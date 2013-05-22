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

    virtual bool isModified() const;
    virtual void setModified(bool modified);

    virtual QString getCurrentFileName() const;
    virtual void setCurrentFileName(const QString &fileName);

protected:
    virtual void saveToStream(QTextStream &out);

private slots:
    void creatorInitialized();
    void switchToSpec();
    void switchToBody();
    void modificationChanged(bool changed);

private:
    CodeCreatorWidget *currentCreator() const;

    QStackedWidget *tab;

    CodeCreatorWidget *specCreatorWidget;
    CodeCreatorWidget *bodyCreatorWidget;

    int initializedCount;
    bool isSpecType;
    
};

#endif // PAIRCODECREATOR_H
