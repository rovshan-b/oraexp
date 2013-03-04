#ifndef DATAEXPORTDIALOG_H
#define DATAEXPORTDIALOG_H

#include <QDialog>
#include "dataexportoptionswidget.h"

class DataExportOptionsWidget;

class DataExportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DataExportDialog(QWidget *parent = 0);
    
    DataExportOptionsWidget *exportWidget() const {return this->exportOptionsWidget;}

public slots:
    virtual void accept();

private:
    void createUi();

    DataExportOptionsWidget *exportOptionsWidget;
};

#endif // DATAEXPORTDIALOG_H
