# mux-sensors-lib

A modular Arduino library for interfacing with analog sensors through CD74HC4067 analog multiplexers. This library provides efficient handling of up to 8 sensor channels using a single ADC pin, reducing hardware complexity and enabling scalable sensor networks.

## Overview

The CD74HC4067 is a 16-channel analog multiplexer/demultiplexer that allows precise control of which input channel is connected to a common output. This library abstracts the multiplexing logic and sensor data processing, providing a clean API for Arduino applications.

## Features

- Support for up to 8 analog sensor channels via single ADC pin
- Modular C architecture with clear separation of concerns
- Configurable normalization ranges for sensor values
- Efficient channel selection using binary addressing
- Comprehensive diagnostic testing suite included
- Low latency channel switching with timing optimization

## Hardware Requirements

- Arduino board (Uno, Mega, or compatible)
- CD74HC4067 analog multiplexer IC
- 3 GPIO pins for address lines (S0, S1, S2)
- 1 analog input pin for signal reading (SIG)
- Appropriate sensor modules for application

## Pin Configuration

Default pin mapping:
- S0 (Address Line 0): GPIO 4
- S1 (Address Line 1): GPIO 5
- S2 (Address Line 2): GPIO 6
- SIG (Signal Output): A0 (analog pin)

These pins can be reconfigured by passing custom pin assignments to `multiplexer_init()`.

## Installation

1. Open the Arduino IDE.
2. Go to **Sketch** > **Include Library** > **Manage Libraries...**
3. Search for "mux-sensors-lib".
4. Click **Install**.

Alternatively, you can download the latest release as a ZIP file and import it via **Sketch** > **Include Library** > **Add .ZIP Library...**.

## Usage

Include the library in your sketch:

```c
#include <mux_sensors_lib.h>
```

### Basic Example

```c
#include <mux_sensors_lib.h>

Multiplexer mux;
Sensor sensor;

int raw_values[8];
int normalized_values[8];

void setup() {
  Serial.begin(9600);

  // Initialize multiplexer and sensor
  mux = multiplexer_init(4, 5, 6, A0);
  sensor = sensor_init(0, 255);

  multiplexer_begin(&mux);
}

void loop() {
  multiplexer_read_all(&mux, raw_values);
  sensor_normalize_buffer(&sensor, raw_values, normalized_values);

  for (int i = 0; i < 8; i++) {
    Serial.print("Channel ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(normalized_values[i]);
  }

  delay(500);
}
```

### Reading a Single Channel

```c
int value = multiplexer_read_channel(&mux, 0);  // Read channel 0
```

### Configuring Normalization Range

```c
Sensor sensor = sensor_init(0, 255);           // Default range 0-255
sensor_set_range(&sensor, 0, 1023);            // Change to 0-1023
```

## Development & Workflow

This project uses standard C tools for quality and testing:

- **Linting**: Automatically formats code on save (using `clang-format`). A GitHub Actions workflow runs a lint check on every push/PR.
- **Testing**: A `Makefile` is provided to run unit tests.
    - Run `make test` to compile and execute all files containing `test` in their name (e.g., `test_sensor.c`).
    - A GitHub Actions workflow also automates testing on every push/PR.

## Testing

Use the included DiagnosticTest example for comprehensive functional validation:
1. Open **File** > **Examples** > **mux-sensors-lib** > **DiagnosticTest**
2. Upload to your Arduino board
3. Open Serial Monitor (9600 baud)
4. Observe diagnostic output and test results

The test suite includes:
- Pin configuration verification
- Single channel read tests
- All channels acquisition timing
- Normalization accuracy validation

## API Reference

### Multiplexer Module

#### Data Structure
```c
typedef struct {
  int pin_s0;
  int pin_s1;
  int pin_s2;
  int pin_sig;
} Multiplexer;
```

#### Functions
- `Multiplexer multiplexer_init(int s0, int s1, int s2, int sig)`: Initialize multiplexer structure
- `void multiplexer_begin(Multiplexer* mux)`: Initialize GPIO pins and set up multiplexer
- `int multiplexer_read_channel(Multiplexer* mux, int channel)`: Read raw ADC value from specified channel
- `void multiplexer_read_all(Multiplexer* mux, int* buffer)`: Read all 8 channels into provided buffer
- `void multiplexer_set_channel(Multiplexer* mux, int channel)`: Set multiplexer to specified channel

### Sensor Module

#### Data Structure
```c
typedef struct {
  int buffer[8];
  int min_value;
  int max_value;
} Sensor;
```

#### Functions
- `Sensor sensor_init(int min_val, int max_val)`: Initialize sensor structure
- `void sensor_set_range(Sensor* sensor, int min_val, int max_val)`: Configure normalization range
- `int sensor_normalize(Sensor* sensor, int raw_value)`: Normalize single value
- `void sensor_normalize_buffer(Sensor* sensor, int* raw_buffer, int* normalized_buffer)`: Batch normalization
- `int sensor_get_value(Sensor* sensor, int index)`: Retrieve stored sensor value
- `void sensor_set_value(Sensor* sensor, int index, int value)`: Store sensor value

## Directory Structure

```
mux-sensors-lib/
├── .clang-format             # Code formatting rules (Google style)
├── .github/workflows/        # CI: lint + test automation
├── library.properties        # Arduino Library Manager metadata
├── keywords.txt              # Syntax highlighting for Arduino IDE
├── Makefile                  # Native C test runner
├── README.md                 # This file
├── LICENSE                   # MIT License
├── examples/
│   ├── BasicRead/            # Minimal usage example
│   │   └── BasicRead.ino
│   └── DiagnosticTest/       # Full diagnostic test suite
│       └── DiagnosticTest.ino
├── src/
│   ├── mux_sensors_lib.h     # Main library header
│   ├── mux/
│   │   ├── multiplexer.h     # Multiplexer API definition
│   │   └── multiplexer.c     # Multiplexer implementation
│   └── sensor/
│       ├── sensor.h          # Sensor API definition
│       └── sensor.c          # Sensor implementation
├── test/
│   └── mocks/
│       └── Arduino.h         # Mock Arduino.h for native testing
└── docs/                     # Documentation and references
```

## Technical Specifications

- ADC Resolution: 10-bit (0-1023)
- Channel Selection Time: Approximately 100 microseconds
- Supported Sensors: Any analog sensor with appropriate voltage range
- Operating Voltage: 5V or 3.3V (multiplexer compatible)
- Maximum Channels per Multiplexer: 8 (S3 connected to GND)

## Performance Considerations

- Channel switching introduces minimal latency (100 microseconds per read)
- Full channel acquisition takes approximately 1 millisecond
- Normalization is performed on-demand to preserve CPU cycles
- Buffer-based operations reduce function call overhead

## Troubleshooting

**Issue: Inconsistent sensor readings**
- Verify multiplexer power supply connections
- Check that address pins are correctly wired
- Confirm analog input is connected to correct multiplexer output pin
- Add small delay between channel selection and reading

**Issue: Serial monitor shows incorrect values**
- Verify baud rate matches (default 9600)
- Ensure normalization range matches expected output
- Check analog sensor connections to multiplexer inputs

**Issue: Library not recognized**
- Verify library installed in correct folder
- Restart Arduino IDE
- Ensure include uses `<mux_sensors_lib.h>` (angle brackets)

## Contributing

Contributions are welcome. Please ensure:
- Code follows existing style conventions
- New features include corresponding test cases
- Documentation is updated alongside code changes

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## References

- CD74HC4067 Datasheet: Available in docs/ folder
- Arduino Analog Reference: https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
