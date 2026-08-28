#include <mux_sensors_lib.h>

// Pin definitions for 74HC4067 multiplexer
const int PIN_S0 = 4;
const int PIN_S1 = 5;
const int PIN_S2 = 6;
const int PIN_SIG = A0;

// Create instances of Multiplexer and Sensor
Multiplexer mux;
Sensor sensor_handler;

// Buffers for sensor data
int raw_values[8];
int normalized_values[8];

void setup() {
    Serial.begin(9600);

    // Initialize multiplexer and sensor
    mux = multiplexer_init(PIN_S0, PIN_S1, PIN_S2, PIN_SIG);
    sensor_handler = sensor_init(0, 255);

    // Begin multiplexer
    multiplexer_begin(&mux);

    Serial.println("Multiplexer initialized successfully");
    Serial.println("Reading from 8 channels...");
}

void loop() {
    // Read all channels from the multiplexer
    multiplexer_read_all(&mux, raw_values);

    // Normalize the readings
    sensor_normalize_buffer(&sensor_handler, raw_values, normalized_values);

    // Display values on serial monitor
    for (int i = 0; i < 8; i++) {
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(normalized_values[i]);
        Serial.print(" (raw: ");
        Serial.print(raw_values[i]);
        Serial.println(")");
    }

    Serial.println("---");
    delay(500);
}