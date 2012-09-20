#ifndef SEQUENCECREATORPANE_H
#define SEQUENCECREATORPANE_H

#include "connection_page/db_object_creator/dbobjectcreatorpane.h"
#include "beans/sequenceinfo.h"

class DbItemListComboBox;
class NameEditor;
class QLineEdit;
class QCheckBox;
class QComboBox;
class MetadataLoader;

class SequenceCreatorPane : public DbObjectCreatorPane
{
    Q_OBJECT
public:
    explicit SequenceCreatorPane(const QString &schemaName, const QString &objectName, QWidget *parent = 0);
    ~SequenceCreatorPane();
    
    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual QString generateCreateDdl();
    virtual QList< QueryListItem > generateAlterDdl();

    virtual bool beforeCreate() {return true;}
    virtual bool beforeAlter();

    virtual QString getSchemaName() const;
    virtual QString getObjectName() const;

public slots:
    void alterQuerySucceeded(const QString &taskName);

private slots:
    void enableControls();

    void sequenceInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);

private:
    SequenceInfo getSequenceInfo() const;
    void setSequenceInfo(SequenceInfo *sequenceInfo);

    void disableControlsForEditMode();
    bool editMode;

    SequenceInfo *originalSequenceInfo;

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
