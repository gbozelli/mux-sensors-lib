#include "sensor.h"

Sensor sensor_init(int min_val, int max_val) {
    Sensor sensor;
    sensor.min_value = min_val;
    sensor.max_value = max_val;

    for (int i = 0; i < SENSOR_NUM_SENSORS; i++) {
        sensor.buffer[i] = 0;
    }

    return sensor;
}

void sensor_set_range(Sensor *sensor, int min_val, int max_val) {
    sensor->min_value = min_val;
    sensor->max_value = max_val;
}

int sensor_normalize(Sensor *sensor, int raw_value) {
    return map(raw_value, 0, 1023, sensor->min_value, sensor->max_value);
}

void sensor_normalize_buffer(Sensor *sensor, int *raw_buffer, int *normalized_buffer) {
    for (int i = 0; i < SENSOR_NUM_SENSORS; i++) {
        normalized_buffer[i] = sensor_normalize(sensor, raw_buffer[i]);
    }
}

int sensor_get_value(Sensor *sensor, int index) {
    if (index >= 0 && index < SENSOR_NUM_SENSORS) {
        return sensor->buffer[index];
    }
    return -1;
}

void sensor_set_value(Sensor *sensor, int index, int value) {
    if (index >= 0 && index < SENSOR_NUM_SENSORS) {
        sensor->buffer[index] = value;
    }
}
