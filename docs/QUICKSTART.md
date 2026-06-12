# Quick Start Guide - mux-sensors-lib

## 5-Minute Setup

### Hardware Connection

Connect your Arduino to the CD74HC4067 multiplexer:

```
Arduino Pin 4  ──→  S0 (Address Line 0)
Arduino Pin 5  ──→  S1 (Address Line 1)
Arduino Pin 6  ──→  S2 (Address Line 2)
Arduino Pin A0 ──→  SIG (Signal Output)
Arduino GND    ──→  GND (Multiplexer Ground)
Arduino 5V     ──→  VCC (Multiplexer Power)

Sensor 0 ──→  Multiplexer Channel 0
Sensor 1 ──→  Multiplexer Channel 1
... (up to 8 sensors)
```

### Testing Setup

1. Open Arduino IDE
2. Copy entire mux-sensors-lib folder to: `Documents/Arduino/libraries/`
3. Restart Arduino IDE
4. Open: File > Examples > mux-sensors-lib > test
5. Select your Arduino board: Tools > Board
6. Select port: Tools > Port
7. Click Upload button
8. Open Serial Monitor: Tools > Serial Monitor
9. Set baud rate to 9600
10. Observe test output

Expected output shows diagnostic information, channel readings, and normalization results.

### Using in Your Project

Create a new sketch:

```c
#include "src/mux/multiplexer.h"
#include "src/sensor/sensor.h"

Multiplexer mux;
Sensor sensor;

int raw[8];
int normalized[8];

void setup() {
  Serial.begin(9600);
  
  // Initialize multiplexer and sensor
  mux = multiplexer_init(4, 5, 6, A0);
  sensor = sensor_init(0, 255);
  
  multiplexer_begin(&mux);
}

void loop() {
  multiplexer_read_all(&mux, raw);
  sensor_normalize_buffer(&sensor, raw, normalized);
  
  for (int i = 0; i < 8; i++) {
    Serial.print("Channel ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(normalized[i]);
  }
  
  delay(100);
}
```

### Common Configurations

**Custom pins:**
```c
mux = multiplexer_init(10, 11, 12, A1);  // Different pins
```

**Custom normalization range:**
```c
Sensor sensor = sensor_init(0, 1023);   // Full 10-bit range
Sensor sensor = sensor_init(-100, 100); // Negative to positive
sensor_set_range(&sensor, 0, 1023);     // Change range
```

**Single channel reading:**
```c
int value = multiplexer_read_channel(&mux, 3);  // Read only channel 3
```

## Troubleshooting

**Serial Monitor shows no data:**
- Check baud rate is 9600
- Verify USB connection
- Restart Arduino IDE

**All channels read 0:**
- Check multiplexer power supply
- Verify S0, S1, S2 pin connections
- Check SIG pin is A0

**Inconsistent readings:**
- Add delay after channel selection
- Check sensor connections
- Verify multiplexer power supply stability

**Upload fails:**
- Select correct board type
- Select correct COM port
- Verify Arduino is detected: Tools > Port

## Documentation Links

- Full README: See README.md
- API Reference: See README.md API Reference section
- Publication Guide: See PUBLISHING.md
- Implementation Details: See IMPLEMENTATION_SUMMARY.md

## Next Steps

1. Test with test.ino
2. Integrate into your project using main.ino as template
3. Modify pin configuration as needed
4. Adjust normalization range for your sensors
5. Deploy to your application

## Support

For issues or questions:
1. Check README.md troubleshooting section
2. Review test.ino diagnostic output
3. Check PUBLISHING.md for library-related questions
4. Consult CD74HC4067 datasheet in docs/
