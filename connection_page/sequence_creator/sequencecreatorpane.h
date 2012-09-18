#ifndef SEQUENCECREATORPANE_H
#define SEQUENCECREATORPANE_H

#include "connection_page/db_object_creator/dbobjectcreatorpane.h"
#include "beans/sequenceinfo.h"

class DbItemListComboBox;
class NameEditor;
class QLineEdit;
class QCheckBox;
class QComboBox;

class SequenceCreatorPane : public DbObjectCreatorPane
{
    Q_OBJECT
public:
    explicit SequenceCreatorPane(const QString &schemaName, const QString &objectName, QWidget *parent = 0);
    
    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual QString generateCreateDdl();
    virtual QList< QueryListItem > generateAlterDdl();

    virtual bool beforeCreate() {return true;}
    virtual bool beforeAlter() {return true;}

    virtual QString getSchemaName() const;
    virtual QString getObjectName() const;

private slots:
    void enableControls();

private:
    SequenceInfo getSequenceInfo() const;
    void setSequenceInfo(SequenceInfo *sequenceInfo);

    void disableControlsForEditMode();
    bool editMode;

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
