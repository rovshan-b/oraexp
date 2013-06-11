#ifndef DMLGENERATORDIALOG_H
#define DMLGENERATORDIALOG_H

#include "codegeneratordialog.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "enums.h"

class QStandardItemModel;
class QComboBox;
class CodeEditor;
class CheckedListBoxWidget;
class QCheckBox;
class QAbstractButton;

class DmlGeneratorDialog : public CodeGeneratorDialog
{
    Q_OBJECT
public:
    explicit DmlGeneratorDialog(DbUiManager *uiManager,
                                const QString &schemaName,
                                const QString &tableName,
                                OraExp::DmlType dmlType,
                                QWidget *parent = 0);

    virtual void createUi();

    virtual void setConnection(DbConnection *db);

protected:
    virtual QString generateCode();

private slots:
    void columnsQueryCompleted(const QueryResult &result);
    void columnFetched(const FetchResult &fetchResult);
    void columnFetchCompleted(const QString &);

    void dmlTypeChanged(int newType);
    
private:
    QString schemaName;
    QString tableName;
    OraExp::DmlType initialDmlType;

    CheckedListBoxWidget *columnsList;
    CheckedListBoxWidget *whereColumnsList;
    QComboBox *dmlTypeCombo;
    QComboBox *bindStyleCombo;
    QCheckBox *includeSchemaCheckBox;

};

#endif // DMLGENERATORDIALOG_H
