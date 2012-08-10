#ifndef INFOPANEL_H
#define INFOPANEL_H

#include <QWidget>

class QToolBar;
class QStackedWidget;

class InfoPanel : public QWidget
{
    Q_OBJECT
public:
    explicit InfoPanel(QWidget *parent = 0);

    void addPanel(QWidget *panel, const QString &title, const QIcon &icon);
    void setCurrentIndex(int index);

private slots:
    void buttonToggled(bool checked);
    
private:
    QToolBar *toolbar;
    QStackedWidget *tab;

    void createToolbar();
    
};

#endif // INFOPANEL_H
