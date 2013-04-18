#ifndef TABLECREATOREXTERNALPROPERTIESGENERALINFOWIDGET_H
#define TABLECREATOREXTERNALPROPERTIESGENERALINFOWIDGET_H

#include <QWidget>
#include <QStringList>
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "beans/tableexternalinfo.h"

class QComboBox;
class DataTableAndToolBarWidget;
class QCheckBox;
class ExtentSizeEditorWidget;
class ComboBoxAndLineEditWidget;
class OracleLoaderDriverPropertiesWidget;
class IQueryScheduler;
class TableCreatorTabs;
class ComboBoxDelegate;
class GenericEditableTableModel;

class TableCreatorExternalPropertiesGeneralInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableCreatorExternalPropertiesGeneralInfoWidget(IQueryScheduler *queryScheduler,
                                                             bool editMode,
                                                             TableCreatorTabs *tableCreator,
                                                             QWidget *parent = 0);

    int selectedDriverIndex() const;

    void setAttributes(TableExternalInfo *info);
    TableExternalInfo itemInfoFromWidgets() const;

    //used in edit mode
    TableExternalInfo *originalExternalInfo;

    void directoryListAvailable(const QStringList &directoryList);

signals:
    void ddlChanged();
    void driverChanged(int selectedIndex);
    void parallelChanged(ComboBoxAndLineEditWidget *parallelEditor);

private slots:
    void parallelEditorChanged();

private:
    TableCreatorTabs *tableCreator;

    QComboBox *externalTableTypeComboBox;
    QComboBox *defaultDirectoryComboBox;
    DataTableAndToolBarWidget *locationsTable;
    GenericEditableTableModel *locationsTableModel;
    ExtentSizeEditorWidget *rejectLimitEditor;
    QComboBox *projectColumnComboBox;
    QCheckBox *usingClobCheckBox;
    ComboBoxAndLineEditWidget *parallelEditor;
    OracleLoaderDriverPropertiesWidget *oracleLoaderProperties;

    void setupLocationsTable();

    //used in edit mode
    void populateLocationsTable();

};

#endif // TABLECREATOREXTERNALPROPERTIESGENERALINFOWIDGET_H
