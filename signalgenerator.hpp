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
    virtual int audioCallback(jack_nframes_t nframes, audioBufVector inBufs,
        audioBufVector outBufs);

    void set_freq(Waveform w, double freq, double volume);
    void set_global_volume(double volume);
    void start_waveform_generation();
    void stop_waveform_generation();

private:
    Waveform m_waveform = Waveform::W_SINE;
    double m_freq = 440;
    double m_volume = 0, m_global_volume = 0.5;

    double m_angle = 0;
    double m_current_sign = 1;
    unsigned int m_pos = 0;

    bool m_playing = true; // unused
};
#endif // SIGNALGENERATOR_HPP
