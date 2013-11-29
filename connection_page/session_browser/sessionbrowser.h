#ifndef SESSIONBROWSER_H
#define SESSIONBROWSER_H

#include <QTimer>
#include "connection_page/connectionpagetab.h"

class SessionListTree;
class SessionBrowserTabs;
class QVBoxLayout;
class QToolBar;
class QSplitter;
class LineEditWithClearButton;
class QComboBox;
class QCheckBox;

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

    void locksOnlyChanged();

private:
    SessionListTree *sessionViewer;
    SessionBrowserTabs *infoTabs;

    QToolBar *toolbar;
    QAction *groupAction;
    LineEditWithClearButton *filterEditor;
    QCheckBox *locksOnlyCheckBox;
    QLabel *statusLabel;
    QAction *progressBarAction;

    QSplitter *splitter;

    QStringList columnTitles;

    void createToolbar(QVBoxLayout *mainLayout);
};

#endif // SESSIONBROWSER_H
