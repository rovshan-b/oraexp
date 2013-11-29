#ifndef CONNECTIONEDITOR_H
#define CONNECTIONEDITOR_H

#include <QWidget>

class QLineEdit;
class QComboBox;
class QCheckBox;
class QStackedWidget;
class DbConnectionInfo;
class QFormLayout;

class ConnectionEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionEditor(QWidget *parent = 0);

    enum ConnectionType
    {
        Direct,
        TNS
    };

    DbConnectionInfo *createConnection();

    void setCurrentConnection(DbConnectionInfo *connectionInfo);
    DbConnectionInfo *getCurrentConnection() const;
    void updateCurrentConnection();

    void deleteCurrentConnection();

    bool validate(bool showErrors = true);

    void focus();

signals:
    void hasConnection(bool has);

private:
    void createUi();
    QWidget *createTnsPane();
    QWidget *createDirectPane();

    void loadTnsList();

    void createTnsNotFoundLabel(const QString &errorMessage);

    void updateConnection(DbConnectionInfo *connection);

    DbConnectionInfo *currentConnection;

    QLineEdit *titleEditor;
    QComboBox *environmentCombo;
    QLineEdit *usernameEditor;
    QLineEdit *passwordEditor;
    QCheckBox *savePasswordCheckBox;
    QComboBox *connectionTypeCombo;
    QStackedWidget *connectionDetailsTab;

    QFormLayout *tnsPaneForm;
    QComboBox *tnsCombo;

    QLineEdit *hostEditor;
    QLineEdit *portEditor;
    QLineEdit *sidEditor;

    QComboBox *connectAsCombo;
    
};

#endif // CONNECTIONEDITOR_H
