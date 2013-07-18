#ifndef RADIOBUTTONGROUP_H
#define RADIOBUTTONGROUP_H

#include <QWidget>

class QBoxLayout;
class QRadioButton;
class QButtonGroup;

class RadioButtonGroup : public QWidget
{
    Q_OBJECT
public:
    explicit RadioButtonGroup(Qt::Orientation orientation, QWidget *parent = 0);

    void addRadioButton(QRadioButton *radio);

    Q_PROPERTY(int currentIndex READ currentIndex)

public slots:
    int currentIndex() const;
    int	checkedId() const;
    void checkRadio(int id);
    
private:
    QBoxLayout *mainLayout;
    QButtonGroup *group;
    
};

#endif // RADIOBUTTONGROUP_H
