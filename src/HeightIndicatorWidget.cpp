#include "include/HeightIndicatorWidget.h"

HeightIndicatorWidget::HeightIndicatorWidget(QWidget *parent) : QWidget(parent)
{
    setAutoFillBackground(true);
    setFixedSize(300, 300);
}

void HeightIndicatorWidget::SetHeight(qint16 height)
{
    m_Height = height;
    update();
}

void HeightIndicatorWidget::paintEvent(QPaintEvent *paintEvent)
{
    Q_UNUSED(paintEvent)

    const int radius = 0.7 * qMin(QWidget::width() / 2.0, QWidget::height() / 2.0);
    const int side = qMin(width(), height());
    const int border = side / 20;

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.translate(QWidget::width() / 2.0, QWidget::height() / 2.0);

    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::NoBrush);

    // Inner circle
    painter.save();
    int innerRadius = radius * 0.5;
    painter.drawEllipse(-innerRadius, -innerRadius, 2 * innerRadius, 2 * innerRadius);
    painter.restore();

    painter.save();
    painter.setFont(QFont{"Arial", radius / 10});
    painter.drawEllipse(-radius + border, -radius + border, 2 * (radius - border), 2 * (radius - border));
    painter.restore();

    painter.save();
    painter.setFont(QFont{"Arial", radius / 10});
    painter.drawText(-radius, -radius + 20, 2 * radius, 2 * radius, Qt::AlignCenter, "Meters");
    painter.restore();

    qint16 hour{0};

    int division_count{50};
    int size_of_hands{15};

    for(int i = 0; i < division_count; ++i)
    {
        if(i % (division_count / 10) == 0)
        {
            painter.drawLine(0, radius - border, 0, radius - border - size_of_hands);

            painter.save();
            painter.setFont(QFont{"Arial", radius / 15});
            painter.drawText(-radius + border, -radius + border - size_of_hands * 1.5, 2 * (radius - border), size_of_hands * 2, Qt::AlignCenter, QString::number(hour));
            painter.restore();
            hour++;
        }
        else
            painter.drawLine(0, radius - border, 0, radius - border - size_of_hands / 2);

        painter.rotate(360.0 / division_count);
    }

    painter.setPen(Qt::NoPen);

    //Hour
    painter.save();
    QPolygon hour_hand;
    hour_hand << QPoint(-0.05 * radius, 0) << QPoint(0.05 * radius, 0) << QPoint(0, -0.6 * radius);
    painter.setBrush(Qt::black);
    double hour_angle = m_Height / 1000.0 * 350.0 / 9.67;
    painter.rotate(hour_angle);
    painter.drawPolygon(hour_hand);
    painter.restore();

    //Minute
    painter.save();
    QPolygon minute_hand;
    minute_hand << QPoint(-0.05 * radius, 0) << QPoint(0.05 * radius, 0) << QPoint(0, -0.8 * radius);
    painter.setBrush(Qt::black);
    double minuteAngle = m_Height / 100.0 * 350.0 / 9.7;
    painter.rotate(minuteAngle);
    painter.drawPolygon(minute_hand);
    painter.restore();

    painter.setPen(Qt::red);
    painter.setBrush(Qt::NoBrush);

    painter.save();
    painter.setFont(QFont{"Arial", radius / 10});
    QRect boundingRect = painter.fontMetrics().boundingRect(QString::number(m_Height));
    int padding = 4;
    int textWidth = boundingRect.width() + 2 / padding;
    int textHeight = boundingRect.height() + 2 / padding;
    boundingRect.adjust(-padding, -padding, padding, padding);
    //painter.drawRect(boundingRect);
    painter.drawText(-textWidth / 2, -textHeight / 2, textWidth, textHeight, Qt::AlignCenter, QString::number(m_Height));
    painter.restore();
}