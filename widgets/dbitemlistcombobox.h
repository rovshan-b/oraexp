#ifndef DBITEMLISTCOMBOBOX_H
#define DBITEMLISTCOMBOBOX_H

#include <QComboBox>
#include "util/param.h"

class QueryResult;
class FetchResult;
class IQueryScheduler;

class DbItemListComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit DbItemListComboBox(const QString &initialValue,
                                const QString &iconName,
                                bool setMaxWidth,
                                bool prependEmptyValue=true,
                                QWidget *parent=0);

    void loadItems(IQueryScheduler *queryScheduler, const QString &queryName, QList<Param*> params=QList<Param*>(),
                   const QString &dbLinkName=QString());

    void setIconColumn(int colNum);

    void setInDelegateMode();

signals:
    void loadingCompleted();

private slots:
    void queryCompleted(const QueryResult &result);
    void itemFetched(const FetchResult &fetchResult);
    void fetchCompleted(const QString &);

private:
    QString iconName;
    bool prependEmptyValue;
    int iconColumn;

    QString currentTxt;
    bool hasSelection;

    bool isInDelegate;

};

#endif // DBITEMLISTCOMBOBOX_H
