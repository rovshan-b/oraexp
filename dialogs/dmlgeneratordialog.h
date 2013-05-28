#ifndef DMLGENERATORDIALOG_H
#define DMLGENERATORDIALOG_H

#include "connection_page/connectionpagewindow.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "enums.h"

class QStandardItemModel;
class QComboBox;
class CodeEditor;
class CheckedListBoxWidget;
class QCheckBox;
class QAbstractButton;

class DmlGeneratorDialog : public ConnectionPageWindow
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

private slots:
    void columnsQueryCompleted(const QueryResult &result);
    void columnFetched(const FetchResult &fetchResult);
    void columnFetchCompleted(const QString &);

    void dmlTypeChanged(int newType);

    void acceptButtonPressed(QAbstractButton *button);
    
private:
    QString schemaName;
    QString tableName;
    OraExp::DmlType initialDmlType;

    CheckedListBoxWidget *columnsList;
    CheckedListBoxWidget *whereColumnsList;
    QComboBox *dmlTypeCombo;
    QComboBox *bindStyleCombo;
    QCheckBox *includeSchemaCheckBox;

    QPushButton *copyToClipboardButton;

    QString generateDml();

};

#endif // DMLGENERATORDIALOG_H
