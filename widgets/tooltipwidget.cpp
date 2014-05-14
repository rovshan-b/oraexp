#include "tooltipwidget.h"

ToolTipWidget::ToolTipWidget(QWidget *parent) : QLabel(parent)
{
    setWindowFlags(Qt::ToolTip);
    hide();
}

void ToolTipWidget::showToolTip(const QPoint &point, const QRect &activeRect)
{
    this->activeRect = activeRect;
    this->initialMouseTracking = parentWidget()->hasMouseTracking();

    parentWidget()->setMouseTracking(true);
    parentWidget()->installEventFilter(this);

    move(point);
    resize(this->sizeHint());
    setVisible(true);
}

bool ToolTipWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseMove && isVisible() && watched == parentWidget()){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(!activeRect.contains(mouseEvent->pos())){
            hideToolTip();
        }
    }

    return QLabel::eventFilter(watched, event);
}

void ToolTipWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    hideToolTip();
}

void ToolTipWidget::hideToolTip()
{
    hide();
    clear();

    parentWidget()->removeEventFilter(this);
    if(parentWidget()->hasMouseTracking() != this->initialMouseTracking){
        parentWidget()->setMouseTracking(this->initialMouseTracking);
    }
}
