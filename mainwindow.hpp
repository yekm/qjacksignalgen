#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "signalgenerator.hpp"
#include <QGraphicsScene>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void freq_change(double f);

    void on_pushButton_clicked();
    void on_verticalSlider_valueChanged(int value);
    void on_radioButton_toggled(bool checked);
    void on_radioButton_2_toggled(bool checked);
    void on_radioButton_3_toggled(bool checked);
    void on_radioButton_4_toggled(bool checked);

private:
    Ui::MainWindow* ui;
    SignalGenerator m_siggen;
    QGraphicsScene m_scene;
    SignalGenerator::Waveform m_current_waveform = SignalGenerator::Waveform::W_SINE;
};

#endif // MAINWINDOW_HPP
