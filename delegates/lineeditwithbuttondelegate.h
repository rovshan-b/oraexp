#ifndef LINEEDITWITHBUTTONDELEGATE_H
#define LINEEDITWITHBUTTONDELEGATE_H

#include <QStyledItemDelegate>

class LineEditWithButton;

class LineEditWithButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit LineEditWithButtonDelegate(bool convertToUpperCase, QObject *parent);

    //void paint(QPainter *painter, const QStyleOptionViewItem &option,
    //                         const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;


    void setTooltip(const QString &tooltip);

    void setAutoConvertToUpperCase(bool convert);

    void setAutoAppendRows(bool autoAppendRows);

protected slots:
    virtual void buttonClicked(LineEditWithButton *senderWidget);
    void editingFinished();

private:
    bool convertToUpperCase;
    QString tooltipText;
    bool autoAppendRows;

};

#endif // LINEEDITWITHBUTTONDELEGATE_H
