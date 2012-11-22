#ifndef SPECBODYSWITCHERWIDGET_H
#define SPECBODYSWITCHERWIDGET_H

#include <QWidget>

class QActionGroup;

class SpecBodySwitcherWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpecBodySwitcherWidget(bool isSpec, QWidget *parent);
    
signals:
    void specBodySwitchRequested();

private slots:
    void specBodySwitcherClicked(QAction *);

private:
    bool isSpec;

    QActionGroup *specBodySwitcherGroup;
    
};

#endif // SPECBODYSWITCHERWIDGET_H
