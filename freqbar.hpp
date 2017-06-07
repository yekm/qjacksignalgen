#ifndef FREQBAR_HPP
#define FREQBAR_HPP

#include <QGraphicsItem>
#include <QObject>

#include "signalgenerator.hpp"

class FreqBar : public QGraphicsItem {
public:
    FreqBar(int freq, SignalGenerator& sg);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;

protected:
    // void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    // void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void wheelEvent(QGraphicsSceneWheelEvent* event);

private:
    const double m_freq;
    SignalGenerator& m_signal_generator;
    double m_volume = 0.5;
    float m_h = 200, m_w = 9;
};

#endif // FREQBAR_HPP
