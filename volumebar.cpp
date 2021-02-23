#include <QPainter>
#include <QDebug>

#include "volumebar.h"

VolumeBar::VolumeBar(QWidget* parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    setMinimumHeight(30);
    setMinimumWidth(200);
}

void VolumeBar::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setPen(Qt::black);
    int left = painter.viewport().left();
    int top = painter.viewport().top();
    int right = painter.viewport().right();
    int bottom = painter.viewport().bottom();
    painter.drawRect(QRect(left + 10,
                           top + 10,
                           right - 20,
                           bottom - 20));

    if (m_level == 0)
        return;

//    int pos = ((right - 20) - (left + 11)) * m_level;
    int pos = ((painter.viewport().right()-20)-(painter.viewport().left()+11))*m_level;
    painter.fillRect(left + 11,
                     top + 10,
                     pos,
                     painter.viewport().height() - 21,
                     Qt::red);
}

void VolumeBar::setLevel(qreal value)
{
    m_level = value;
    update();
}
