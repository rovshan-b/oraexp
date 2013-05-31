#ifndef TABLECREATORPARTITIONSTAB_H
#define TABLECREATORPARTITIONSTAB_H

#include <QWidget>
#include "interfaces/istringlistretriever.h"
#include "enums.h"
#include "connectivity/fetchresult.h"
#include "beans/partitioninginfo.h"
#include "models/partitionsmodel.h"
#include "widgets/datatable.h"
#include "datatableandtoolbarwidget.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"

class IQueryScheduler;
class QComboBox;
class LineEditWithButton;
class DataTableAndToolBarWidget;
class QLineEdit;
class TableCreatorTabs;
class IdentifierNameDelegate;
class TableCreatorPartitions;
class ComboBoxDelegate;
class GenericEditableTableModel;

class TableCreatorPartitionsTab : public QWidget, public IStringListRetriever
{
    Q_OBJECT
public:
    explicit TableCreatorPartitionsTab(TableCreatorTabs* tableCreator,
                                       OraExp::PartitionPart configureFor,
                                       bool configureForIndex,
                                       QWidget *parent);

    int getPartitioningStrategy() const;
    void setPartitioningStrategy(int strategyIx, bool enableControl);
    OraExp::PartitionType getPartitionType() const;
    void setPartitionType(OraExp::PartitionType pType);

    void setPartitionColumns(const QString &columns);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    void tablespaceListAvailable();

    void setCreatorMode(DbObjectCreator::CreatorMode creatorMode);
    void setInterval(const QString &interval);

    void addToOriginalPartitionList(const FetchResult &fetchResult);

    void setPartitionNameRetriever(IStringListRetriever *partitionNameRetriever);

    virtual QStringList getStringList(const QString &type=QString());

    QString generateDdl() const;
    QString generatePartitionListDdl(TableCreatorPartitionsTab *subpartitionsTab=0, const QString &parentPartitionName=QString()) const;

    //edit mode variables and functions
    void populateTableWithPartitionList(const PartitioningInfo &partitionginInfo, int frozenRowIx=-1);
    void loadFromPartitioningInfo(const PartitioningInfo &pInfo, int frozenRowIx=-1);
    PartitioningInfo partitioningInfoFromWidgets(bool includeIncorrectRows=true) const;

    void removeIncorrectRows();

    //PartitioningInfo *getOriginalPartitionginInfo(){return &originalPartitionginInfo;}

    void alterQuerySucceeded(const QString &taskName, PartitioningInfo &partitionginInfo);

    //void setOriginalPartitioningInfo(const PartitioningInfo &pInfo){this->originalPartitionginInfo=pInfo;}

signals:
    void ddlChanged() const;
    void partitioningStrategyChanged(int partitioningStrategyIx);
    void partitionTypeChanged(int selPartitionType);

public slots:
    void partitionTypeComboChanged(int selPartitionType);

    bool normalizeTableData(bool prompt=false);

private slots:
    void hashPartitionSpecifiedByChanged(int specifiedByIx);

    void toUpper(const QString &text);
    void browseColumnList(LineEditWithButton *lineEditWithButton);
    void browseTablespaceList(LineEditWithButton *lineEditWithButton);

    void partitionsTabActivated();

    void tableDataChanged(const QModelIndex &from, const QModelIndex &to);

private:
    //TableCreatorTabs* tableCreator;
    TableCreatorPartitions *parentTab;
    OraExp::PartitionPart configureFor;

    IStringListRetriever *foreignKeyRetriever;

    QComboBox *partitioningStrategy;
    QComboBox *partitionType;
    LineEditWithButton *columnSelector;
    QComboBox *specifiedBySelector;
    QLineEdit *partitionCountEditor;
    QComboBox *intervalSelector;
    QComboBox *foreignKeySelector;
    LineEditWithButton *storeInSelector;

    DataTableAndToolBarWidget *table;
    PartitionsModel *partitionListModel;
    IdentifierNameDelegate *partitionNameDelegate;
    ComboBoxDelegate *valuesDelegate;
    ComboBoxDelegate *tablespaceDelegate;

    void adjustTable();

    OraExp::PartitionType currentPartitionType;

    bool configureForIndex;

    bool supportsIntervalAndReferencePartitioning;

    //edit mode variables and functions
    DbObjectCreator::CreatorMode creatorMode;
    void disableColumnsForFrozenRows(int rowIx);
    //PartitioningInfo originalPartitionginInfo;
};

#endif // TABLECREATORPARTITIONSTAB_H
