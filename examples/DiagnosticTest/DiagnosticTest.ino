// DiagnosticTest.ino
// Complete test suite for the mux-sensors-lib library
// Designed to test all functionality on Arduino IDE

#include <mux_sensors_lib.h>

// Pin definitions for 74HC4067 multiplexer
const int PIN_S0 = 4;
const int PIN_S1 = 5;
const int PIN_S2 = 6;
const int PIN_SIG = A0;

// Library instances
Multiplexer mux;
Sensor sensor_handler;

// Sensor data buffers
int raw_channels[MUX_NUM_CHANNELS];
int normalized_channels[MUX_NUM_CHANNELS];

// Test state
int test_case = 0;
unsigned long last_test_time = 0;
const unsigned long TEST_INTERVAL = 3000;  // 3 seconds between tests

// Function declarations
void run_diagnostic_test();
void run_single_channel_test();
void run_all_channels_test();
void run_normalization_test();
void print_sensor_data(int* data, const char* label);
void print_status_message(const char* message);

void setup() {
    Serial.begin(9600);

    // Initialize library components
    mux = multiplexer_init(PIN_S0, PIN_S1, PIN_S2, PIN_SIG);
    sensor_handler = sensor_init(0, 255);
    multiplexer_begin(&mux);

    print_status_message("=== MUX-SENSORS-LIB DIAGNOSTIC TEST ===");
    Serial.print("Multiplexer pins: S0=");
    Serial.print(PIN_S0);
    Serial.print(", S1=");
    Serial.print(PIN_S1);
    Serial.print(", S2=");
    Serial.print(PIN_S2);
    Serial.print(", SIG=A");
    Serial.println(PIN_SIG - A0);
    print_status_message("Starting diagnostic tests...");
}

void loop() {
    // Run tests sequentially every TEST_INTERVAL milliseconds
    if (millis() - last_test_time >= TEST_INTERVAL) {
        last_test_time = millis();

        switch (test_case) {
            case 0:
                run_diagnostic_test();
                break;
            case 1:
                run_single_channel_test();
                break;
            case 2:
                run_all_channels_test();
                break;
            case 3:
                run_normalization_test();
                break;
            default:
                // Cycle back to start
                test_case = -1;
                print_status_message("\n=== TEST CYCLE COMPLETE ===");
                print_status_message("Restarting test sequence...\n");
        }
        test_case++;
    }
}

// Test 1: Diagnostic - verify pin configuration
void run_diagnostic_test() {
    print_status_message("\n[TEST 1] DIAGNOSTIC - Pin Configuration");

    Serial.print("Address pins configured: S0=");
    Serial.print(PIN_S0);
    Serial.print(", S1=");
    Serial.print(PIN_S1);
    Serial.print(", S2=");
    Serial.println(PIN_S2);

    Serial.print("Signal pin (SIG): A");
    Serial.println(PIN_SIG - A0);

    // Test each address pin by selecting channels that exercise each bit
    for (int ch = 0; ch < MUX_NUM_CHANNELS; ch++) {
        multiplexer_set_channel(&mux, ch);
        Serial.print("Channel ");
        Serial.print(ch);
        Serial.println(" selected OK");
    }

    print_status_message("Diagnostic test complete.");
}

// Test 2: Single channel read test
void run_single_channel_test() {
    print_status_message("\n[TEST 2] SINGLE CHANNEL READ");

    for (int ch = 0; ch < MUX_NUM_CHANNELS; ch++) {
        int value = multiplexer_read_channel(&mux, ch);
        Serial.print("Channel ");
        Serial.print(ch);
        Serial.print(": ");
        Serial.print(value);
        Serial.println(" (0-1023 ADC)");
    }

    print_status_message("Single channel read complete.");
}

// Test 3: All channels read test with timing
void run_all_channels_test() {
    print_status_message("\n[TEST 3] ALL CHANNELS - Rapid Read");

    unsigned long start_time = micros();
    multiplexer_read_all(&mux, raw_channels);
    unsigned long elapsed = micros() - start_time;

    Serial.print("Acquisition time: ");
    Serial.print(elapsed);
    Serial.println(" microseconds");

    print_sensor_data(raw_channels, "Raw ADC Values");

    print_status_message("All channels test complete.");
}

// Test 4: Normalization test
void run_normalization_test() {
    print_status_message("\n[TEST 4] NORMALIZATION");

    Serial.print("Normalization range: ");
    Serial.print(sensor_handler.min_value);
    Serial.print(" - ");
    Serial.println(sensor_handler.max_value);

    multiplexer_read_all(&mux, raw_channels);
    sensor_normalize_buffer(&sensor_handler, raw_channels, normalized_channels);

    print_sensor_data(raw_channels, "Raw ADC Values (0-1023)");
    print_sensor_data(normalized_channels, "Normalized Values (0-255)");

    // Test changing range dynamically
    sensor_set_range(&sensor_handler, 0, 100);
    sensor_normalize_buffer(&sensor_handler, raw_channels, normalized_channels);
    print_sensor_data(normalized_channels, "Re-normalized Values (0-100)");

    // Restore original range
    sensor_set_range(&sensor_handler, 0, 255);

    print_status_message("Normalization test complete.");
}

// Helper: Print sensor data array
void print_sensor_data(int* data, const char* label) {
    Serial.print(">> ");
    Serial.println(label);

    for (int i = 0; i < MUX_NUM_CHANNELS; i++) {
        Serial.print("   CH");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(data[i]);
    }
}

// Helper: Print status messages
void print_status_message(const char* message) { Serial.println(message); }
