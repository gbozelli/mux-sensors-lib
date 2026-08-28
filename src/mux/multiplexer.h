#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(ARDUINO) && ARDUINO > 0
#include <Arduino.h>
#else
#include "Arduino.h"
#endif

#define MUX_NUM_CHANNELS 8
#define MUX_ADC_MAX 1023
#define MUX_ADC_MIN 0
#define MUX_SETTLING_TIME 100

typedef struct {
    int pin_s0;
    int pin_s1;
    int pin_s2;
    int pin_sig;
} Multiplexer;

Multiplexer multiplexer_init(int s0, int s1, int s2, int sig);
void multiplexer_begin(Multiplexer* mux);
int multiplexer_read_channel(Multiplexer* mux, int channel);
void multiplexer_read_all(Multiplexer* mux, int* buffer);
void multiplexer_set_channel(Multiplexer* mux, int channel);

#ifdef __cplusplus
}
#endif

#endif  // MULTIPLEXER_H
