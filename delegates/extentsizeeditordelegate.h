#ifndef EXTENTSIZEEDITORDELEGATE_H
#define EXTENTSIZEEDITORDELEGATE_H

#include <QStyledItemDelegate>

class ExtentSizeEditorDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ExtentSizeEditorDelegate(QObject *parent);
    
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;

private slots:
    void editorDataChanged();
};

#endif // EXTENTSIZEEDITORDELEGATE_H
