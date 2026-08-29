# mux-sensors-lib

> 🇧🇷 **Aviso para Brasileiros e Iniciantes:** Temos documentação traduzida e detalhada para ajudar você! Leia o [Guia Rápido (QUICKSTART) em Português](docs/QUICKSTART_pt-BR.md) e o nosso [Guia de Contribuição](CONTRIBUTING.md).

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

Multiplexer mux;       // Struct that stores the multiplexer pin config
Sensor sensor;         // Struct that stores the normalization range

int raw_values[8];         // Array to hold raw ADC readings (0-1023)
int normalized_values[8];  // Array to hold normalized values

void setup() {
  Serial.begin(9600);

  // Create the multiplexer: pins S0=4, S1=5, S2=6, signal=A0
  mux = multiplexer_init(4, 5, 6, A0);

  // Create the sensor with normalization range 0-255
  sensor = sensor_init(0, 255);

  // Set up the GPIO pins (must be called once in setup)
  multiplexer_begin(&mux);  // &mux = "address of mux" (pointer)
}

void loop() {
  // Read all 8 channels and store in raw_values[]
  multiplexer_read_all(&mux, raw_values);

  // Convert raw readings (0-1023) to normalized range (0-255)
  sensor_normalize_buffer(&sensor, raw_values, normalized_values);

  // Print each channel value
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
// Read only channel 0 (returns a value between 0 and 1023)
int value = multiplexer_read_channel(&mux, 0);
```

### Configuring Normalization Range

```c
// Create sensor with range 0-255 (default)
Sensor sensor = sensor_init(0, 255);

// Later, change the range to 0-1023 if needed
sensor_set_range(&sensor, 0, 1023);
```

## Development & Workflow

This project uses two automated checks that run on every push and pull request:

### Code Formatting (clang-format)

All `.c`, `.h`, and `.ino` files must follow the formatting rules defined in `.clang-format`. If the CI check fails, you can fix it by running:

```bash
# Check which files have formatting issues
find . -name '*.c' -o -name '*.h' -o -name '*.ino' | xargs clang-format --dry-run

# Auto-fix all files
find . -name '*.c' -o -name '*.h' -o -name '*.ino' | xargs clang-format -i
```

### Unit Tests (make test)

The project includes C tests that run on your computer (without Arduino hardware). To run them:

```bash
make test
```

If you want to add new tests, edit `src/test/test.c` — the file has comments explaining the pattern.

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

> 🇧🇷 **Iniciante?** Nós temos um guia passo-a-passo ensinando como fazer um fork, usar o git e abrir seu primeiro Pull Request. Leia o **[CONTRIBUTING.md](CONTRIBUTING.md)**.

Contributions are welcome! Before submitting a pull request, please check:

1. **Formatting**: Run `clang-format -i` on your files (or the CI will fail)
2. **Tests pass**: Run `make test` and make sure all tests pass
3. **Comments**: Add comments to any new code explaining what it does
4. **Documentation**: Update this README if your change adds new functions

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## References

- CD74HC4067 Datasheet: Available in docs/ folder
- Arduino Analog Reference: https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
