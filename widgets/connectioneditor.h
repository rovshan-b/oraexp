#ifndef CONNECTIONEDITOR_H
#define CONNECTIONEDITOR_H

#include <QWidget>

class QLineEdit;
class QComboBox;
class QStackedWidget;

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

private:
    void createUi();
    QWidget *createTnsPane();
    QWidget *createDirectPane();

    QLineEdit *titleEditor;
    QComboBox *environmentCombo;
    QLineEdit *usernameEditor;
    QLineEdit *passwordEditor;
    QComboBox *connectionTypeCombo;
    QStackedWidget *connectionDetailsTab;

    QComboBox *tnsCombo;

    QLineEdit *hostEditor;
    QLineEdit *portEditor;
    QLineEdit *sidEditor;

    QComboBox *connectAsCombo;
    
};

#endif // CONNECTIONEDITOR_H
