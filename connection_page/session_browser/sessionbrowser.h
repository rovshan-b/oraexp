#ifndef SESSIONBROWSER_H
#define SESSIONBROWSER_H

#include "connection_page/connectionpagetab.h"

class SessionListTree;
class QVBoxLayout;
class QToolBar;
class QSplitter;
class LineEditWithClearButton;

class SessionBrowser : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit SessionBrowser(DbUiManager *uiManager, QWidget *parent = 0);
    
    virtual void createUi();

    virtual void setConnection(DbConnection *db);

    virtual void beforeClose();

private slots:
    void startProgress();
    void stopProgress();

    void setInProgress(bool inProgress);

    void changeGroupingColumns();

    void headerReady(const QStringList &columnTitles);
    void dataReady();

    void changeSplitDirection(QAction *action);

    void filterChanged(const QString &newFilter);

private:
    SessionListTree *sessionViewer;

    QToolBar *toolbar;
    QAction *groupAction;
    LineEditWithClearButton *filterEditor;
    QLabel *statusLabel;
    QAction *progressBarAction;

    QSplitter *splitter;

    QStringList columnTitles;

    void createToolbar(QVBoxLayout *mainLayout);
};

#endif // SESSIONBROWSER_H
