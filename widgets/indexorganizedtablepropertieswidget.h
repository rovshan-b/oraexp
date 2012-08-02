#ifndef INDEXORGANIZEDTABLEPROPERTIESWIDGET_H
#define INDEXORGANIZEDTABLEPROPERTIESWIDGET_H

#include <QWidget>
#include "beans/storageparams.h"
#include "beans/indexorganizedtableproperties.h"

class QLineEdit;
class QComboBox;
class ComboBoxAndLineEditWidget;
class QCheckBox;
class LineEditWithButton;
class IQueryScheduler;
class IStringListRetriever;

class IndexOrganizedTablePropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IndexOrganizedTablePropertiesWidget(IQueryScheduler *queryScheduler,
                                                 IStringListRetriever *columnListRetriever,
                                                 bool isEditMode,
                                                 QWidget *parent = 0);

    IndexOrganizedTableProperties getAttributes() const;
    void setAttributes(const IndexOrganizedTableProperties &attributes,
                       const IndexOrganizedTableProperties &originalAttributes);

private slots:
    void storageParamsButtonClicked(LineEditWithButton *lineEditWithButton);
    void enableOverflowPropertiesWidgets(bool enable);

private:
    QLineEdit *pctThresholdEditor;
    QComboBox *mappingTableComboBox;
    ComboBoxAndLineEditWidget *keyCompressionEditor;
    QCheckBox *overflowCheckBox;
    QComboBox *includingComboBox;
    LineEditWithButton *storageEditor;
    QComboBox *loggingComboBox;

    StorageParams storageParams;

    IQueryScheduler *queryScheduler;
    IStringListRetriever *columnListRetriever;

    bool isEditMode;

    //used only in edit mode
    bool originallyHadOverflow;
    IndexOrganizedTableProperties originalAttributes;
};

#endif // INDEXORGANIZEDTABLEPROPERTIESWIDGET_H
