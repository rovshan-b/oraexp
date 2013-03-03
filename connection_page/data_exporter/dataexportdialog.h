#ifndef DATAEXPORTDIALOG_H
#define DATAEXPORTDIALOG_H

#include <QDialog>

class DataExportOptionsWidget;

class DataExportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DataExportDialog(QWidget *parent = 0);
    
private:
    void createUi();

    DataExportOptionsWidget *exportOptionsWidget;
};

#endif // DATAEXPORTDIALOG_H
