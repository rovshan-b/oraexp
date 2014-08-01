#ifndef CODESTRUCTURESORTFILTERMODEL_H
#define CODESTRUCTURESORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class CodeStructureSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CodeStructureSortFilterModel(QObject *parent = 0);
    
protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    
};

#endif // CODESTRUCTURESORTFILTERMODEL_H
