#include "include/HeightIndicatorWidget.h"

HeightIndicatorWidget::HeightIndicatorWidget(QWidget *parent) : QWidget(parent)
{
    setAutoFillBackground(true);
    setMinimumSize(100, 100);
}

void HeightIndicatorWidget::SetHeight(qint16 height)
{
    m_Height = height;
    update();
}

void HeightIndicatorWidget::paintEvent(QPaintEvent *paintEvent)
{
    Q_UNUSED(paintEvent)

    QPainter painter(this);

    int radius = 1.0 * qMin(QWidget::width() / 2.0, QWidget::height() / 2.0);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.translate(QWidget::width() / 2.0, QWidget::height() / 2.0);

    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::NoBrush);

    painter.save();
    painter.drawEllipse(-radius, -radius, 2 * radius, 2 * radius);
    painter.restore();

    painter.save();
    int innerRadius = radius * 0.6;
    painter.drawEllipse(-innerRadius, -innerRadius, 2 * innerRadius, 2 * innerRadius);
    painter.restore();

    painter.save();
    painter.setFont(QFont{"Arial", radius / 10});
    painter.drawText(-radius, -radius + 10, 2 * radius, 2 * radius, Qt::AlignCenter, QString::number(m_Height));
    painter.restore();

    painter.save();
    painter.setFont(QFont{"Arial", radius / 10});
    painter.drawText(-radius, -radius + 20, 2 * radius, 2 * radius, Qt::AlignCenter, "Meters");
    painter.restore();

    qint16 hour{0};

    for(int i = 0; i < 60; ++i)
    {
        if(i % 6 == 0)
        {
            painter.drawLine(0, radius, 0, radius - 10);
            painter.rotate(180);
            painter.drawText(QRectF(-20, radius - 30, 40, 20), Qt::AlignCenter, QString::number(hour));
            ++hour;
            painter.rotate(-180);
        }

        else
            painter.drawLine(0, radius, 0, radius - 5);

        painter.rotate(6.0);
    }

    painter.setPen(Qt::NoPen);

    //Hour
    painter.save();
    QPolygon hour_hand;
    hour_hand << QPoint(-0.05 * radius, 0) << QPoint(0.05 * radius, 0) << QPoint(0, -0.6 * radius);
    painter.setBrush(Qt::black);
    painter.rotate(m_Height / 30);
    painter.drawPolygon(hour_hand);
    painter.restore();

    //Minute
    painter.save();
    QPolygon minute_hand;
    minute_hand << QPoint(-0.05 * radius, 0) << QPoint(0.05 * radius, 0) << QPoint(0, -0.8 * radius);
    painter.setBrush(Qt::black);
    painter.rotate(m_Height);
    painter.drawPolygon(minute_hand);
    painter.restore();
}