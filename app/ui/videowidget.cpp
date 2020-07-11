#include "videowidget.h"
#include <QKeyEvent>
#include <QMouseEvent>

VideoWidget::VideoWidget(QWidget *parent):
    QVideoWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QPalette p = this->palette();
    p.setColor(QPalette::Window, Qt::black);
    this->setPalette(p);
    this->setAttribute(Qt::WA_OpaquePaintEvent);
}

void VideoWidget::toggleFullScreenMode()
{
    this->setFullScreen(!this->isFullScreen());
}

void VideoWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::MiddleButton)
    {
        emit pauseRequestInFullscreenMode();
    }
}

void VideoWidget::keyReleaseEvent(QKeyEvent *event)
{
    if((event->key() == Qt::Key_Escape || event->key() == Qt::Key_F) &&
       this->isFullScreen())
    {
        this->setFullScreen(false);
        event->accept();
    }
    else if(event->key() == Qt::Key_Space)
    {
        emit pauseRequestInFullscreenMode();
    }
    else
    {
        QVideoWidget::keyReleaseEvent(event);
    }
}

void VideoWidget::mousePressEvent(QMouseEvent *event)
{
     QVideoWidget::mousePressEvent(event);
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    toggleFullScreenMode();
    event->accept();
}

void VideoWidget::wheelEvent(QWheelEvent *event)
{
    emit volumeLevelChanged(event->delta());
    QVideoWidget::wheelEvent(event);
}
