#ifndef INFOPANEL_H
#define INFOPANEL_H

#include <QObject>
#include <QIcon>

class QToolBar;
class QStackedWidget;
class QBoxLayout;
class QLabel;
class QAction;
class InfoPane;

class InfoPanel : public QObject
{
    Q_OBJECT
public:
    explicit InfoPanel(QWidget *parent);

    void addPane(InfoPane *pane, const QString &title, const QIcon &icon);
    void addInfoWidget(QWidget *w);

    void setCurrentIndex(int index);
    int getCurrentIndex() const;

    QToolBar *getToolbar() const {return this->toolbar;}
    QStackedWidget *getPanel() const {return this->tab;}

    bool isPanelVisible() const;

    void setCurrentPane(InfoPane *pane);
    int indexOf(InfoPane *pane) const;

    void closePane(InfoPane *pane);

public slots:
    void closePanel();

private slots:
    void buttonToggled(bool checked);

private:
    void createToolbar();

    QToolBar *toolbar;
    QAction *placeholderAction;

    QStackedWidget *tab;
    QList<InfoPane*> panes;

    QBoxLayout *createPanelHeader(const QString &title);
};

#endif // INFOPANEL_H
