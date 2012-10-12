#ifndef DBLINKCREATORPANE_H
#define DBLINKCREATORPANE_H

#include "connection_page/db_object_creator/dbobjectcreatorsimplepane.h"

class DbItemListComboBox;
class NameEditor;
class QLineEdit;
class QCheckBox;
class QComboBox;
class DbLinkInfo;
class PasswordEditor;

class DbLinkCreatorPane : public DbObjectCreatorSimplePane
{
    Q_OBJECT
public:
    explicit DbLinkCreatorPane(DbObjectCreator *objectCreator, QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);
    
    virtual QString getSchemaName() const;
    virtual QString getObjectName() const;

    virtual QString generateCreateDdl();
    virtual QList< QueryListItem > generateAlterDdl();

    virtual bool beforeCreate() const;
    virtual bool beforeAlter() const;

public slots:
    void alterQuerySucceeded(const QString &taskName);

protected:
    virtual QLayout *createForm();
    DbLinkInfo getDbLinkInfo() const;
    void setObjectInfo(DbObjectInfo *objectInfo);
    DbItemListComboBox *schemaListCombo() const {return 0;}

private slots:
    void enableControls();

private:
    //DbItemListComboBox *schemaList;
    NameEditor *dblinkNameEditor;
    QCheckBox *publicCheckBox;
    QComboBox *targetDbComboBox;
    QCheckBox *currentUserCheckBox;
    QLineEdit *usernameEditor;
    PasswordEditor *passwordEditor;

    QCheckBox *sharedCheckBox;
    QLineEdit *sharedUsernameEditor;
    PasswordEditor *sharedPasswordEditor;
};

#endif // DBLINKCREATORPANE_H
