#ifndef SEQUENCECREATORPANE_H
#define SEQUENCECREATORPANE_H

#include "connection_page/db_object_creator/dbobjectcreatorsimplepane.h"

class DbItemListComboBox;
class NameEditor;
class QLineEdit;
class QCheckBox;
class QComboBox;
class SequenceInfo;

class SequenceCreatorPane : public DbObjectCreatorSimplePane
{
    Q_OBJECT
public:
    explicit SequenceCreatorPane(DbObjectCreator *objectCreator, QWidget *parent = 0);

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
    virtual void disableControlsForEditMode();
    SequenceInfo getSequenceInfo() const;
    void setObjectInfo(DbObjectInfo *objectInfo);
    DbItemListComboBox *schemaListCombo() const {return this->schemaList;}

private slots:
    void enableControls();

private:
    DbItemListComboBox *schemaList;
    NameEditor *sequenceNameEditor;
    QLineEdit *minValueEditor;
    QLineEdit *maxValueEditor;
    QLineEdit *incrementByEditor;
    QCheckBox *cycleCheckbox;
    QCheckBox *orderedCheckbox;
    QComboBox *cacheComboBox;
    QLineEdit *cacheSizeEditor;
    QLineEdit *startWithEditor;
    
};

#endif // SEQUENCECREATORPANE_H
