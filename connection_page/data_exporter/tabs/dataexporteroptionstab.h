#ifndef DATAEXPORTEROPTIONSTAB_H
#define DATAEXPORTEROPTIONSTAB_H

#include "connection_page/data_operation/tabs/dataoperationoptionstab.h"
#include "../dataexportoptionswidget.h"

class DataExporterOptionsTab : public DataOperationOptionsTab
{
    Q_OBJECT
public:
    explicit DataExporterOptionsTab(QWidget *parent);

    virtual DbObjectComparisonOptions *getOptions() const;

    DataExportOptionsWidget *getOptionsWidget() const {return this->optionsWidget;}

protected:
    virtual void createOptionsPane(QVBoxLayout *layout);

private slots:
    void exportFormatChanged(DataExporterBase::ExportFormat newFormat);

private:
    DataExportOptionsWidget *optionsWidget;

    QCheckBox *singleFileCheckBox;
    
};

#endif // DATAEXPORTEROPTIONSTAB_H
