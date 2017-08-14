#include "signalgenerator.hpp"

#include <cmath>
#include <limits>

#include <QDebug>

SignalGenerator::SignalGenerator()
    : JackCpp::AudioIO("qjacksignalgen", 0, 2)
{
}

void SignalGenerator::put_value_to_buffer(jack_default_audio_sample_t v, JackCpp::AudioIO::audioBufVector &outBufs)
{
    if (m_left)
        outBufs[1][buffer_index] = v;
    else
        outBufs[1][buffer_index] = 0;
    if (m_right)
        outBufs[0][buffer_index] = v;
    else
        outBufs[0][buffer_index] = 0;
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

    jack_default_audio_sample_t value;

    switch (m_waveform) {
    case Waveform::W_SINE:
        for (buffer_index = 0; buffer_index < nframes; buffer_index++) {
            value = std::sin(m_angle) * current_volume;
            put_value_to_buffer(value, outBufs);
            m_angle += delta;
            if (almost_equal(m_angle, 200 * M_PI, 5)) // 5?
                m_angle = 0;
        }
        break;

    // TODO: rewrite everything below

    case Waveform::W_SQUARE:
        for (buffer_index = 0; buffer_index < nframes; buffer_index++) {
            value = m_current_sign * current_volume;
            put_value_to_buffer(value, outBufs);
            ++m_pos;
            if (m_pos > samples_per_hz / 2) {
                m_pos = 0;
                m_current_sign *= -1;
            }
        }
        break;

    case Waveform::W_TRIANGLE:
        for (buffer_index = 0; buffer_index < nframes; buffer_index++) {
            value = (m_current_sign - m_pos * delta_linear * m_current_sign) * current_volume;
            put_value_to_buffer(value, outBufs);
            ++m_pos;
            if (m_pos > samples_per_hz / 2) {
                m_pos = 0;
                m_current_sign *= -1;
            }
        }
        break;

    case Waveform::W_SAWTOOTH:
        for (buffer_index = 0; buffer_index < nframes; buffer_index++) {
            value = (-1 + m_pos * delta_linear) * current_volume;
            put_value_to_buffer(value, outBufs);
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

void SignalGenerator::enable_right(bool enable)
{
    m_right = enable;
}

void SignalGenerator::enable_left(bool enable)
{
    m_left = enable;
}
