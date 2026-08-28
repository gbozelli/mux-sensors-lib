#include "multiplexer.h"

Multiplexer multiplexer_init(int s0, int s1, int s2, int sig) {
    Multiplexer mux;
    mux.pin_s0 = s0;
    mux.pin_s1 = s1;
    mux.pin_s2 = s2;
    mux.pin_sig = sig;
    return mux;
}

void multiplexer_begin(Multiplexer* mux) {
    pinMode(mux->pin_s0, OUTPUT);
    pinMode(mux->pin_s1, OUTPUT);
    pinMode(mux->pin_s2, OUTPUT);

    digitalWrite(mux->pin_s0, LOW);
    digitalWrite(mux->pin_s1, LOW);
    digitalWrite(mux->pin_s2, LOW);
}

void multiplexer_set_channel(Multiplexer* mux, int channel) {
    if (channel < 0 || channel >= MUX_NUM_CHANNELS) {
        return;
    }

    digitalWrite(mux->pin_s0, (channel & 0x01));       // Bit 0 (S0)
    digitalWrite(mux->pin_s1, (channel & 0x02) >> 1);  // Bit 1 (S1)
    digitalWrite(mux->pin_s2, (channel & 0x04) >> 2);  // Bit 2 (S2)

    delayMicroseconds(MUX_SETTLING_TIME);
}

int multiplexer_read_channel(Multiplexer* mux, int channel) {
    multiplexer_set_channel(mux, channel);
    return analogRead(mux->pin_sig);
}

void multiplexer_read_all(Multiplexer* mux, int* buffer) {
    for (int i = 0; i < MUX_NUM_CHANNELS; i++) {
        buffer[i] = multiplexer_read_channel(mux, i);
    }
}
