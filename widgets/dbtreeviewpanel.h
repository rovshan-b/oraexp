#ifndef DBTREEVIEWPANEL_H
#define DBTREEVIEWPANEL_H

#include <QWidget>
#include "dbtreeview.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class DbTreeView;
class QPushButton;
class QBoxLayout;
class SelectLinkedObjectsDialog;
class QStandardItemModel;

class DbTreeViewPanel : public QWidget
{
    Q_OBJECT
public:
    explicit DbTreeViewPanel(QWidget *parent = 0);

    DbTreeView *tree() const {return this->dbTree;}
    
private slots:
    void selectAll();
    void selectNone();
    void selectLinkedObjects();

    void treeReady();

    void childrenLoaded();
    void childrenLoadError(const OciException &exception);

    void linkedObjectsQueryCompleted(const QueryResult &result);
    void linkedObjectRecordAvailable(const FetchResult &fetchResult);
    void linkedObjectsFetchCompleted(const QString &);

private:
    DbTreeView *dbTree;
    QPushButton *btnSelectAll;
    QPushButton *btnSelectNone;
    QPushButton *btnSelectLinkedObjects;

    SelectLinkedObjectsDialog *selectLinkedObjectsDialog;

    void createActionButtons(QBoxLayout *layout);
    void setInProgress(bool inProgress);
    void selectInTreeView(QStandardItemModel *tableModel);

    //variables related to find linked objects action
    bool findOnlyTables;
    QList<DbTreeModel::DbTreeNodeType> availableObjectTypes;
    
};

#endif // DBTREEVIEWPANEL_H
