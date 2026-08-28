#ifndef SENSOR_H
#define SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(ARDUINO) && ARDUINO > 0
#include <Arduino.h>
#else
#include "Arduino.h"
#endif

#define SENSOR_NUM_SENSORS 8

typedef struct {
    int buffer[SENSOR_NUM_SENSORS];
    int min_value;
    int max_value;
} Sensor;

Sensor sensor_init(int min_val, int max_val);
void sensor_set_range(Sensor* sensor, int min_val, int max_val);
int sensor_normalize(Sensor* sensor, int raw_value);
void sensor_normalize_buffer(Sensor* sensor, int* raw_buffer, int* normalized_buffer);
int sensor_get_value(Sensor* sensor, int index);
void sensor_set_value(Sensor* sensor, int index, int value);

#ifdef __cplusplus
}
#endif

#endif  // SENSOR_H
