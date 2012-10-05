#ifndef SYNONYMCREATORPANE_H
#define SYNONYMCREATORPANE_H

#include "connection_page/db_object_creator/dbobjectcreatorsimplepane.h"

class DbItemListComboBox;
class NameEditor;
class QLineEdit;
class QCheckBox;
class DbItemListComboBox;
class SynonymInfo;
class LineEditWithButton;

class SynonymCreatorPane : public DbObjectCreatorSimplePane
{
    Q_OBJECT
public:
    explicit SynonymCreatorPane(DbObjectCreator *objectCreator, QWidget *parent = 0);

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
    virtual void disableControlsForEditMode();
    SynonymInfo getSynonymInfo() const;
    void setObjectInfo(DbObjectInfo *objectInfo);
    DbItemListComboBox *schemaListCombo() const {return this->schemaList;}

private slots:
    void enableControls();
    void showTargetObjectList();

private:
    QCheckBox *publicCheckBox;
    DbItemListComboBox *schemaList;
    NameEditor *synonymNameEditor;

    QCheckBox *isOverDbLinkCheckBox;
    DbItemListComboBox *dbLinkComboBox;
    DbItemListComboBox *ownerComboBox;
    LineEditWithButton *objectNameEditor;
};

#endif // SYNONYMCREATORPANE_H
