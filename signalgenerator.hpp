#ifndef SIGNALGENERATOR_HPP
#define SIGNALGENERATOR_HPP

#include "jackaudioio.hpp"

class SignalGenerator : public JackCpp::AudioIO {
public:
    enum class Waveform {
        W_SINE,
        W_SQUARE,
        W_TRIANGLE,
        W_SAWTOOTH,
    };

    SignalGenerator();
    void put_value_to_buffer(jack_default_audio_sample_t v, audioBufVector & outBufs);
    virtual int audioCallback(jack_nframes_t nframes, audioBufVector inBufs,
        audioBufVector outBufs);

    void set_freq(Waveform w, double freq, double volume);
    void set_global_volume(double volume);
    void start_waveform_generation();
    void stop_waveform_generation();
    void enable_right(bool enable);
    void enable_left(bool enable);

private:
    Waveform m_waveform = Waveform::W_SINE;
    double m_freq = 440;
    double m_volume = 0, m_global_volume = 0.5;

    double m_angle = 0;
    double m_current_sign = 1;
    unsigned int m_pos = 0;

    bool m_playing = true; // unused
    unsigned int buffer_index = 0;
    bool m_left = true, m_right = true;
};
#endif // SIGNALGENERATOR_HPP
