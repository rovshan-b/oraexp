#ifndef COLUMNSELECTORDELEGATE_H
#define COLUMNSELECTORDELEGATE_H

#include "lineeditwithbuttondelegate.h"

class IStringListRetriever;
class LineEditWithButton;

class ColumnSelectorDelegate : public LineEditWithButtonDelegate
{
    Q_OBJECT
public:
    explicit ColumnSelectorDelegate(IStringListRetriever *columnNameRetriever, const QString &dialogTitle, QObject *parent, bool convertToUpperCase=true);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    IStringListRetriever *getColumnNameRetriever() const{return columnNameRetriever;}

private:
    IStringListRetriever *columnNameRetriever;

    QString dialogTitle;

private slots:
    virtual void buttonClicked(LineEditWithButton *senderWidget);

};

#endif // COLUMNSELECTORDELEGATE_H
