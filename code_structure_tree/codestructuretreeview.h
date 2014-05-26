#ifndef CODESTRUCTURETREEVIEW_H
#define CODESTRUCTURETREEVIEW_H

#include <QTreeView>

class CodeStructureTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit CodeStructureTreeView(QWidget *parent = 0);

    QModelIndex findFirstVisibleParent(const QModelIndex &child) const;
    
};

#endif // CODESTRUCTURETREEVIEW_H
