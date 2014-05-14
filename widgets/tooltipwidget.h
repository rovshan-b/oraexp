#ifndef TOOLTIPWIDGET_H
#define TOOLTIPWIDGET_H

#include <QtGui>

class ToolTipWidget : public QLabel
{
    Q_OBJECT
public:
    ToolTipWidget(QWidget *parent);

    void showToolTip(const QPoint &point, const QRect &activeRect);

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);
    virtual void mousePressEvent ( QMouseEvent * event );

private:
    QRect activeRect;
    bool initialMouseTracking;

    void hideToolTip();
};

#endif // TOOLTIPWIDGET_H
