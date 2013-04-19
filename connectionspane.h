#ifndef CONNECTIONSPANE_H
#define CONNECTIONSPANE_H

#include <QTabWidget>

class DbConnection;
class ConnectionPage;

class ConnectionsPane : public QTabWidget
{
    Q_OBJECT
public:
    explicit ConnectionsPane(QWidget *parent = 0);

    void addConnection(DbConnection *db);
    ConnectionPage *currentConnectionPage() const;

signals:
    void connectionPaneStateChanged();
    void connectionPageStateChanged();

public slots:
    void closeTab(int index);
    void popOutTab();
    void ctrlTabPressed();

protected:
    void contextMenuEvent(QContextMenuEvent* pEvent);

private:
    void showTabBar();

};

#endif // CONNECTIONSPANE_H
