#ifndef CONNECTIONSELECTORWIDGET_H
#define CONNECTIONSELECTORWIDGET_H

#include "lineeditwithbutton.h"

class DbConnection;

class ConnectionSelectorWidget : public LineEditWithButton
{
    Q_OBJECT
public:
    explicit ConnectionSelectorWidget(QWidget *parent = 0);
    ~ConnectionSelectorWidget();

    DbConnection *getDb() const {return this->db;}

signals:
    void connectionEstablished(DbConnection *db);

private slots:
    void connectToServer();

private:
    DbConnection *db;

    void cleanup();

};

#endif // CONNECTIONSELECTORWIDGET_H
