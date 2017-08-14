#include "mainwindow.hpp"
#include "freqbar.hpp"
#include "ui_mainwindow.h"

#include <cmath>
#include <memory>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{ parent }
    , ui{ new Ui::MainWindow }
{
    ui->setupUi(this);
    /*
    for (int f = 10, x = 0;
         f <= 22000;
         f += std::pow(10, std::floor(std::log10(f))) / 3, ++x) {
        //auto item = std::make_unique<FreqBar>(i, m_siggen);
        auto* item = new FreqBar(f, m_siggen);
        item->setPos(QPointF(x * 10, 0));
        //QObject::connect(item, &FreqBar::activated, this, &MainWindow::on_freq_activate);
        m_scene.addItem(item);
    }*/
    //ui->graphicsView->setScene(&m_scene);
    connect(ui->widget, &FreqWidget::currentFreq, this, &MainWindow::freq_change);
}

MainWindow::~MainWindow()
{
    m_siggen.stop();
    m_siggen.close();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (m_siggen.getState() == JackCpp::AudioIO::active) {
        ui->pushButton->setText("Play");
        m_siggen.stop();
        //m_siggen.connectToPhysical(0, 0);
        //m_siggen.connectToPhysical(1, 1);
    } else {
        //m_siggen.disconnectInPort(0);
        //m_siggen.disconnectOutPort(1);
        ui->pushButton->setText("Stop");
        m_siggen.start();
    }
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    m_siggen.set_global_volume(static_cast<double>(value) / ui->verticalSlider->maximum());
}

void MainWindow::freq_change(double f)
{
    ui->label->setText(QString::number(f) + " Hz");
    m_siggen.set_freq(m_current_waveform, f, 1);
    ui->horizontalSlider->setValue(f);
}

void MainWindow::on_radioButton_toggled(bool checked)
{
    if (checked)
        m_current_waveform = SignalGenerator::Waveform::W_SINE;
}

void MainWindow::on_radioButton_2_toggled(bool checked)
{
    if (checked)
        m_current_waveform = SignalGenerator::Waveform::W_SQUARE;
}

void MainWindow::on_radioButton_3_toggled(bool checked)
{
    if (checked)
        m_current_waveform = SignalGenerator::Waveform::W_TRIANGLE;
}

void MainWindow::on_radioButton_4_toggled(bool checked)
{
    if (checked)
        m_current_waveform = SignalGenerator::Waveform::W_SAWTOOTH;
}

void MainWindow::on_pushButton_left_clicked(bool checked)
{
    m_siggen.enable_left(checked);
}

void MainWindow::on_pushButton_right_clicked(bool checked)
{
    m_siggen.enable_right(checked);
}
