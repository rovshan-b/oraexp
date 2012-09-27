#ifndef SYNONYMCREATORPANE_H
#define SYNONYMCREATORPANE_H

#include "connection_page/db_object_creator/dbobjectcreatorsimplepane.h"

class DbItemListComboBox;
class NameEditor;
class QLineEdit;
class QCheckBox;
class QComboBox;
class SynonymInfo;

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

public slots:
    void alterQuerySucceeded(const QString &taskName);

protected:
    virtual QLayout *createForm();
    virtual void disableControlsForEditMode();
    SynonymInfo getSynonymInfo() const;
    void setObjectInfo(DbObjectInfo *objectInfo);
    DbItemListComboBox *schemaListCombo() const {return this->schemaList;}


private:
    DbItemListComboBox *schemaList;
    NameEditor *synonymNameEditor;
};

#endif // SYNONYMCREATORPANE_H
