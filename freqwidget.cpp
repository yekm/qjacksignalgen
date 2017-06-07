#include "freqwidget.hpp"

#include <QBrush>
#include <QPaintEvent>
#include <QPainter>

#include <QDebug>

#include <cmath>

FreqWidget::FreqWidget(QWidget* parent)
    : QWidget(parent)
{
}

double FreqWidget::scale(double f)
{
    const auto w = width();
    return (std::log10(f - m_min_freq) - 1) / (std::log10(m_max_freq) - 1) * w;
}

double FreqWidget::unscale(double p)
{
    const auto w = width();
    return std::pow(10, p * (std::log10(m_max_freq) - 1) / w + 1) + m_min_freq;
}

void FreqWidget::paintEvent(QPaintEvent*)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine, Qt::RoundCap));

    for (int f = m_min_freq;
         f <= m_max_freq;
         f += std::pow(10, std::floor(std::log10(f)))) {
        auto pos = scale(f);
        painter.drawLine(pos, 0, pos, height());
        painter.drawText(pos, height() - 20, 30, 10, 0, QString::number(f));
        //qDebug() << "drwafreq " << f << " at " << pos;
    }

    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));

    auto pos = scale(m_current_freq);
    painter.drawLine(pos, 0, pos, height());
    painter.drawText(pos, m_mouse_pos.y() - 20, 50, 10, 0,
        QString::number(m_current_freq));

    painter.end();
}

void FreqWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_mouse_pos = event->pos();
    m_current_freq = unscale(m_mouse_pos.x());
    emit(currentFreq(m_current_freq));
    update();
}

void FreqWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_1:
        m_current_freq = 100;
        break;
    case Qt::Key_2:
        m_current_freq = 200;
        break;
    case Qt::Key_3:
        m_current_freq = 300;
        break;
    case Qt::Key_4:
        m_current_freq = 400;
        break;
    case Qt::Key_5:
        m_current_freq = 500;
        break;
    case Qt::Key_6:
        m_current_freq = 1000;
        break;
    case Qt::Key_7:
        m_current_freq = 2000;
        break;
    case Qt::Key_8:
        m_current_freq = 3000;
        break;
    case Qt::Key_9:
        m_current_freq = 4000;
        break;
    case Qt::Key_0:
        m_current_freq = 8000;
        break;
    case Qt::Key_Minus:
        m_current_freq = 9000;
        break;
    case Qt::Key_Equal:
        m_current_freq = 10000;
        break;
    }
    emit(currentFreq(m_current_freq));
    update();
}

void FreqWidget::keyReleaseEvent(QKeyEvent* event)
{
    m_current_freq = unscale(m_mouse_pos.x());
    emit(currentFreq(m_current_freq));
    update();
}
