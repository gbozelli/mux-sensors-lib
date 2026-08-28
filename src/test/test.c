/**
 * test.c - Example test file for mux-sensors-lib
 *
 * This is a skeleton for unit tests that run natively with gcc
 * (not on Arduino hardware). It uses the mock Arduino.h from
 * test/mocks/ to stub out hardware functions.
 *
 * Build & run: make test
 *
 * TODO: Implement real unit tests for multiplexer and sensor modules.
 */

#include <stdio.h>

#include "multiplexer.h"
#include "sensor.h"

static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT_EQ(expected, actual, msg)                                                         \
    do {                                                                                         \
        if ((expected) == (actual)) {                                                            \
            tests_passed++;                                                                      \
        } else {                                                                                 \
            tests_failed++;                                                                      \
            printf("  FAIL: %s (expected %d, got %d)\n", (msg), (int)(expected), (int)(actual)); \
        }                                                                                        \
    } while (0)

void test_multiplexer_init(void) {
    printf("[TEST] multiplexer_init\n");

    Multiplexer mux = multiplexer_init(4, 5, 6, 14);  // 14 = A0 in mock
    ASSERT_EQ(4, mux.pin_s0, "pin_s0 should be 4");
    ASSERT_EQ(5, mux.pin_s1, "pin_s1 should be 5");
    ASSERT_EQ(6, mux.pin_s2, "pin_s2 should be 6");
    ASSERT_EQ(14, mux.pin_sig, "pin_sig should be A0 (14)");
}

void test_sensor_init(void) {
    printf("[TEST] sensor_init\n");

    Sensor sensor = sensor_init(0, 255);
    ASSERT_EQ(0, sensor.min_value, "min_value should be 0");
    ASSERT_EQ(255, sensor.max_value, "max_value should be 255");
}

void test_sensor_set_range(void) {
    printf("[TEST] sensor_set_range\n");

    Sensor sensor = sensor_init(0, 255);
    sensor_set_range(&sensor, 10, 100);
    ASSERT_EQ(10, sensor.min_value, "min_value should be 10 after set_range");
    ASSERT_EQ(100, sensor.max_value, "max_value should be 100 after set_range");
}

int main(void) {
    printf("=== mux-sensors-lib unit tests ===\n\n");

    test_multiplexer_init();
    test_sensor_init();
    test_sensor_set_range();

    printf("\n--- Results: %d passed, %d failed ---\n", tests_passed, tests_failed);

    return tests_failed > 0 ? 1 : 0;
}
