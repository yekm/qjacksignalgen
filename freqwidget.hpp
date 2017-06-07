#ifndef FREQWIDGET_HPP
#define FREQWIDGET_HPP

#include <QPoint>
#include <QWidget>

class FreqWidget : public QWidget {
    Q_OBJECT
public:
    explicit FreqWidget(QWidget* parent = nullptr);

    double scale(double f);
    double unscale(double p);

signals:
    void currentFreq(double);

public slots:

protected:
    void paintEvent(QPaintEvent*) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    QPoint m_mouse_pos;
    double m_max_freq = 22000, m_min_freq = 10;
    double m_current_freq = 440;
};

#endif // FREQWIDGET_HPP
