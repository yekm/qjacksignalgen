#include "signalgenerator.hpp"

#include <cmath>
#include <limits>

#include <QDebug>

SignalGenerator::SignalGenerator()
    : JackCpp::AudioIO("qjacksignalgen", 0, 2)
{
}

template <class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
almost_equal(T x, T y, int ulp)
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::abs(x - y) < std::numeric_limits<T>::epsilon() * std::abs(x + y) * ulp
        // unless the result is subnormal
        || std::abs(x - y) < std::numeric_limits<T>::min();
}

int SignalGenerator::audioCallback(jack_nframes_t nframes, JackCpp::AudioIO::audioBufVector inBufs, JackCpp::AudioIO::audioBufVector outBufs)
{
    const double current_volume = m_volume * m_global_volume;
    const double sr = getSampleRate();
    const double delta = 2 * M_PI / sr * m_freq;
    const double delta_linear = 4 / (sr / m_freq);
    const double samples_per_hz = sr / m_freq;

    switch (m_waveform) {
    case Waveform::W_SINE:
        for (unsigned int j = 0; j < nframes; j++) {
            outBufs[1][j] = outBufs[0][j] = std::sin(m_angle) * current_volume;
            m_angle += delta;
            if (almost_equal(m_angle, 200 * M_PI, 5)) // 5?
                m_angle = 0;
        }
        break;

    // TODO: rewrite everything below

    case Waveform::W_SQUARE:
        for (unsigned int j = 0; j < nframes; j++) {
            outBufs[1][j] = outBufs[0][j] = m_current_sign * current_volume;
            ++m_pos;
            if (m_pos > samples_per_hz / 2) {
                m_pos = 0;
                m_current_sign *= -1;
            }
        }
        break;

    case Waveform::W_TRIANGLE:
        for (unsigned int j = 0; j < nframes; j++) {
            outBufs[1][j] = outBufs[0][j] = (m_current_sign - m_pos * delta_linear * m_current_sign) * current_volume;
            ++m_pos;
            if (m_pos > samples_per_hz / 2) {
                m_pos = 0;
                m_current_sign *= -1;
            }
        }
        break;

    case Waveform::W_SAWTOOTH:
        for (unsigned int j = 0; j < nframes; j++) {
            outBufs[1][j] = outBufs[0][j] = (-1 + m_pos * delta_linear) * current_volume;
            ++m_pos;
            if (m_pos > samples_per_hz) {
                m_pos = 0;
            }
        }
        break;

    default:
        throw std::logic_error("Usupported waveform function.");
        break;
    }

    return 0;
}

void SignalGenerator::set_freq(SignalGenerator::Waveform w, double freq, double volume)
{
    m_waveform = w;
    m_freq = freq;
    m_volume = volume;
}

void SignalGenerator::set_global_volume(double volume)
{
    m_global_volume = volume;
}

void SignalGenerator::start_waveform_generation()
{
    m_playing = true;
}

void SignalGenerator::stop_waveform_generation()
{
    m_playing = false;
}
