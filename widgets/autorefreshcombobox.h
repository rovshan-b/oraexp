#ifndef AUTOREFRESHCOMBOBOX_H
#define AUTOREFRESHCOMBOBOX_H

#include <QComboBox>
#include <QTimer>

class AutoRefreshComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit AutoRefreshComboBox(QWidget *receiver);
    
private slots:
    void refreshIntervalChanged(const QString &newInterval);

private:
    QTimer timer;
    
};

#endif // AUTOREFRESHCOMBOBOX_H
