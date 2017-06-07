#include "freqbar.hpp"

#include <QBrush>
#include <QGraphicsSceneWheelEvent>
#include <QPainter>
#include <QPen>
#include <QStyle>
#include <QStyleOptionGraphicsItem>

#include <QDebug>

#include <cmath>

FreqBar::FreqBar(int freq, SignalGenerator& sg)
    : m_freq(freq)
    , m_signal_generator(sg)
{
    qDebug() << "Freq " << freq;
    setAcceptHoverEvents(true);
}

QRectF FreqBar::boundingRect() const
{
    return QRectF(0, 0, m_w, m_h);
}

QPainterPath FreqBar::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, m_w, m_h);
    return path;
}

void FreqBar::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);

    QColor fillColor{ Qt::green };
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(120);

    painter->setBrush(fillColor);
    painter->drawRect(0, 0, m_w, m_h - std::ceil(m_h * m_volume));
    fillColor = fillColor.dark(180);
    painter->setBrush(fillColor);
    painter->drawRect(0, m_h - m_h * m_volume, m_w, std::ceil(m_h * m_volume));
}

void FreqBar::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    m_signal_generator.set_freq(SignalGenerator::Waveform::W_SINE, m_freq, m_volume);
    m_signal_generator.start_waveform_generation();

    QGraphicsItem::hoverEnterEvent(event);
}

void FreqBar::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    m_signal_generator.stop_waveform_generation();
    QGraphicsItem::hoverEnterEvent(event);
}

void FreqBar::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    double divider = 2000.;
    if (event->modifiers() & Qt::ShiftModifier)
        divider *= 4.;
    if (event->modifiers() & Qt::ControlModifier)
        divider *= 2.;

    m_volume += event->delta() / divider;
    if (m_volume > 1)
        m_volume = 1;
    if (m_volume < 0)
        m_volume = 0;
    m_signal_generator.set_freq(SignalGenerator::Waveform::W_SINE, m_freq, m_volume);
    QGraphicsItem::wheelEvent(event);
    update();
}
