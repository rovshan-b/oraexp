#ifndef INFOPANEL_H
#define INFOPANEL_H

#include <QObject>
#include <QIcon>
#include <QKeySequence>

class QToolBar;
class QStackedWidget;
class QBoxLayout;
class QLabel;
class QAction;

class InfoPanel : public QObject
{
    Q_OBJECT
public:
    explicit InfoPanel(QWidget *parent, bool decreaseFontSize=true);

    void addPane(QWidget *pane, const QString &title, const QIcon &icon, const QKeySequence &shortcut=QKeySequence());
    void addInfoWidget(QWidget *w);

    void setCurrentIndex(int index);
    int getCurrentIndex() const;

    QToolBar *getToolbar() const {return this->toolbar;}
    QStackedWidget *getPanel() const {return this->tab;}

    bool isPanelVisible() const;

    void setCurrentPane(QWidget *pane);
    int indexOf(QWidget *pane) const;

    void closePane(QWidget *pane);

public slots:
    void closePanel();

private slots:
    void buttonToggled(bool checked);

private:
    void createToolbar();

    bool decreaseFontSize;

    QToolBar *toolbar;
    QAction *placeholderAction;

    QStackedWidget *tab;
    QList<QWidget*> panes;

    QBoxLayout *createPanelHeader(const QString &title);
};

#endif // INFOPANEL_H
