#ifndef INFOPANEL_H
#define INFOPANEL_H

#include <QObject>
#include <QIcon>

class QToolBar;
class QStackedWidget;
class QBoxLayout;

class InfoPanel : public QObject
{
    Q_OBJECT
public:
    explicit InfoPanel(QWidget *parent = 0);

    void addPanel(QWidget *panel, const QString &title, const QIcon &icon);
    void setCurrentIndex(int index);
    int getCurrentIndex() const;

    QToolBar *getToolbar() const {return this->toolbar;}
    QStackedWidget *getPanel() const {return this->tab;}

    bool isPanelVisible() const;

public slots:
    void closePanel();

private slots:
    void buttonToggled(bool checked);

private:
    QToolBar *toolbar;
    QStackedWidget *tab;

    QBoxLayout *createPanelHeader(const QString &title);
};

#endif // INFOPANEL_H
