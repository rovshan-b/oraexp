#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>

class DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidget(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0);

    bool isActive() const {return this->active;}

private slots:
    void stateChanged(bool active);
    
private:
    bool active;
    
};

#endif // DOCKWIDGET_H
