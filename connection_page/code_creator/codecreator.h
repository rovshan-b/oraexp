#ifndef CODECREATOR_H
#define CODECREATOR_H

#include "connection_page/connectionpagetab.h"
#include "navtree/dbtreemodel.h"
#include "codecreatorwidget.h"

class CodeCreator : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit CodeCreator(
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

    virtual QString getCurrentFileName(int childIndex = 0) const;
    virtual void setCurrentFileName(const QString &fileName, int childIndex = 0);

protected:
    virtual void saveToStream(QTextStream &out, int childIndex = 0);

private slots:
    void modificationChanged(bool changed);

private:
    CodeCreatorWidget *creatorWidget;
};

#endif // CODECREATOR_H
