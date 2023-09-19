#ifndef HEIGHT_INDICATOR_WIDGET_H
#define HEIGHT_INDICATOR_WIDGET_H

#include <QWidget>
#include <QPainter>
#include <cmath>

class HeightIndicatorWidget : public QWidget
{
Q_OBJECT

private:
    qint16 m_Height{0};
public:
    explicit HeightIndicatorWidget(QWidget *parent = nullptr);

    void SetHeight(qint16 height);

protected:
    void paintEvent(QPaintEvent *paintEvent) override;
};

#endif //HEIGHT_INDICATOR_WIDGET_H
