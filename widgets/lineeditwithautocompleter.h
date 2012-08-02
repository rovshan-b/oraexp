#ifndef LINEEDITWITHAUTOCOMPLETER_H
#define LINEEDITWITHAUTOCOMPLETER_H

#include <QLineEdit>
#include <QStringList>

class IStringListRetriever;


class LineEditWithAutocompleter : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEditWithAutocompleter(QWidget *parent = 0);

    void setItemListRetriever(IStringListRetriever *itemListRetriever);
    IStringListRetriever *getItemListRetriever() const;

    void setItemList(const QStringList &itemList);

protected:
    virtual void keyReleaseEvent(QKeyEvent * event);

private:
    IStringListRetriever *itemListRetriever;
    QStringList itemList;
};

#endif // LINEEDITWITHAUTOCOMPLETER_H
