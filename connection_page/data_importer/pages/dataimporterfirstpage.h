#ifndef DATAIMPORTERFIRSTPAGE_H
#define DATAIMPORTERFIRSTPAGE_H

#include <QWizardPage>

class IQueryScheduler;
class DbItemListComboBox;
class FileSelectorWidget;

class DataImporterFirstPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DataImporterFirstPage(
            const QString &schemaName,
            const QString &tableName,
            QWidget *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual bool validatePage();

private slots:
    void loadTableList();
    
private:
    QString schemaName;
    QString tableName;

    DbItemListComboBox *schemaList;
    DbItemListComboBox *tableList;
    FileSelectorWidget *fileSelector;

    IQueryScheduler *queryScheduler;

    QString getSchemaName() const;

    QString lastLoadedSchemaName;

};

#endif // DATAIMPORTERFIRSTPAGE_H
