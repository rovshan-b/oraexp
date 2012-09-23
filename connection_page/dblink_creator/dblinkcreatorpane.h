#ifndef DBLINKCREATORPANE_H
#define DBLINKCREATORPANE_H

#include "connection_page/db_object_creator/dbobjectcreatorsimplepane.h"

class DbItemListComboBox;
class NameEditor;
class QLineEdit;
class QCheckBox;
class QComboBox;
class DbLinkInfo;
class LineEditAndCheckBoxWidget;

class DbLinkCreatorPane : public DbObjectCreatorSimplePane
{
    Q_OBJECT
public:
    explicit DbLinkCreatorPane(DbObjectCreator *objectCreator, QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);
    
    virtual QString getSchemaName() const;
    virtual QString getObjectName() const;

    virtual bool beforeAlter() const;

    virtual QString generateCreateDdl();
    virtual QList< QueryListItem > generateAlterDdl();

public slots:
    void alterQuerySucceeded(const QString &taskName);

protected:
    virtual QLayout *createForm();
    DbLinkInfo getDbLinkInfo() const;
    void setObjectInfo(DbObjectInfo *objectInfo);
    DbItemListComboBox *schemaListCombo() const {return 0;}

private slots:
    void enableControls();
    void showPassword();

private:
    //DbItemListComboBox *schemaList;
    NameEditor *dblinkNameEditor;
    QCheckBox *publicCheckBox;
    QComboBox *targetDbComboBox;
    QCheckBox *currentUserCheckBox;
    QLineEdit *usernameEditor;
    LineEditAndCheckBoxWidget *passwordEditor;

    QCheckBox *sharedCheckBox;
    QLineEdit *sharedUsernameEditor;
    LineEditAndCheckBoxWidget *sharedPasswordEditor;
};

#endif // DBLINKCREATORPANE_H
