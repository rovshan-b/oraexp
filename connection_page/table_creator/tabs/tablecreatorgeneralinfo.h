#ifndef TABLECREATORGENERALINFO_H
#define TABLECREATORGENERALINFO_H

#include "../tablecreatortab.h"
#include "beans/storageparams.h"
#include "beans/tableadditionalattributes.h"
#include "beans/indexorganizedtableproperties.h"
#include "connectivity/fetchresult.h"
#include "connectivity/queryresult.h"
#include "util/param.h"
#include "enums.h"
#include "defines.h"
#include "beans/tableinfo.h"

class DbItemListComboBox;
class NameEditor;
class QPlainTextEdit;
class LineEditWithButton;
class QComboBox;
class IStringListRetriever;

class TableCreatorGeneralInfo : public TableCreatorTab
{
    Q_OBJECT
public:
    explicit TableCreatorGeneralInfo(const QString &schemaName,
                                     const QString &tableName,
                                     TableCreatorTabs *objectCreator,
                                     DbObjectCreator::CreatorMode creatorMode,
                                     QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setColumnListRetriever(IStringListRetriever *columnListRetriever);

    TableGeneralInfo getTableGeneralInfo() const;

    QString getSchema() const;
    QString getTableName() const;
    void setTableName(const QString &tableName);

    OraExp::TableType getTableType() const;

    void disableControlsForEditMode();

    void setTableInfo(TableInfo *tableInfo);
signals:
    void ddlChanged() const;
    void currentSchemaChanged();
    void tableTypeChanged(OraExp::TableType newTableType);

protected:
     bool eventFilter(QObject *obj, QEvent *event);

private:
    QComboBox *tableTypeComboBox;
    DbItemListComboBox *schemaList;
    NameEditor *tableNameEditor;
    QPlainTextEdit *tableCommentEditor;
    LineEditWithButton *storageParamsEditor;
    LineEditWithButton *additionalAttributesEditor;
    LineEditWithButton *indexOrganizedTablePropertiesEditor;

    StorageParams storageParams;
    TableAdditionalAttributes additionalAttributes;
    IndexOrganizedTableProperties indexOrganizedProperties;

    IStringListRetriever *columnListRetriever;

    //these variables are only used in edit mode
    TableGeneralInfo *originalGeneralInfo;

protected slots:
    void storageParamsButtonClicked(LineEditWithButton *);
    void additionalAttributesButtonClicked(LineEditWithButton *);
    void indexOrganizedPropertiesButtonClicked(LineEditWithButton *);
    void tableTypeComboBoxChanged(int newTableType);

    void alterQuerySucceeded(const QString &taskName);
    //void alterQueryError(const QString &/*taskName*/){}

};

#endif // TABLECREATORGENERALINFO_H
