#ifndef STORAGEPARAMSWIDGET_H
#define STORAGEPARAMSWIDGET_H

#include <QWidget>
#include "beans/storageparams.h"
#include "enums.h"

class QComboBox;
class QLineEdit;
class QCheckBox;
class ExtentSizeEditorWidget;
class TablespaceListComboBox;
class IQueryScheduler;

class StorageParamsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StorageParamsWidget(IQueryScheduler *queryScheduler,
                                 bool isEditMode,
                                 bool configureForIndex=true,
                                 bool configureForLob=false,
                                 QWidget *parent=0);

    StorageParams getStorageParams() const;
    void setStorageParams(const StorageParams &params);

private:
    TablespaceListComboBox *tablespaceCombo;
    QLineEdit *pctFreeEditor;
    QLineEdit *pctUsedEditor;
    QLineEdit *initTransEditor;
    QLineEdit *maxTransEditor;

    ExtentSizeEditorWidget *initExtent;
    ExtentSizeEditorWidget *nextExtent;
    QLineEdit *pctIncreaseEditor;
    QLineEdit *minExtentsEditor;
    ExtentSizeEditorWidget *maxExtentsWidget;

    ExtentSizeEditorWidget *maxSizeWidget;

    QLineEdit *freeListsEditor;
    QLineEdit *freeListGroupsEditor;

    QComboBox *bufferPoolCombo;

    QString getSizeUnitText(OraExp::ExtentUnit unit) const;

};

#endif // STORAGEPARAMSWIDGET_H
