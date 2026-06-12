# Pure C Implementation Notes

This version of mux-sensors-lib has been converted from C++ to pure C for maximum portability and compatibility across different compilers and platforms.

## Key Changes from C++ to C

### Data Structures

C++ Classes → C Structs:
```c
// Multiplexer
typedef struct {
  int pin_s0;
  int pin_s1;
  int pin_s2;
  int pin_sig;
} Multiplexer;

// Sensor
typedef struct {
  int buffer[8];
  int min_value;
  int max_value;
} Sensor;
```

### Methods → Functions

C++ Methods → C Functions with Pointers:
```c
// C++ style
mux.begin();

// C style
multiplexer_begin(&mux);
```

### Initialization

C++ Constructors → C Initialization Functions:
```c
// C++ style
Multiplexer mux(4, 5, 6, A0);
Sensor sensor(0, 255);

// C style
Multiplexer mux = multiplexer_init(4, 5, 6, A0);
Sensor sensor = sensor_init(0, 255);
```

## API Patterns

### Multiplexer Module

```c
// Initialize
Multiplexer mux = multiplexer_init(s0, s1, s2, sig);

// Setup GPIO
multiplexer_begin(&mux);

// Read single channel
int value = multiplexer_read_channel(&mux, channel);

// Read all channels
multiplexer_read_all(&mux, buffer);

// Set channel manually
multiplexer_set_channel(&mux, channel);
```

### Sensor Module

```c
// Initialize
Sensor sensor = sensor_init(min_val, max_val);

// Change range
sensor_set_range(&sensor, min_val, max_val);

// Normalize single value
int normalized = sensor_normalize(&sensor, raw_value);

// Normalize buffer
sensor_normalize_buffer(&sensor, raw_buffer, normalized_buffer);

// Get/set values
int value = sensor_get_value(&sensor, index);
sensor_set_value(&sensor, index, value);
```

## Advantages of Pure C

1. **Portability**: Compiles on any platform with a C compiler
2. **Performance**: No C++ overhead, minimal memory footprint
3. **Compatibility**: Works with Arduino IDE, PlatformIO, and standard C toolchains
4. **Simplicity**: Easier to understand and debug
5. **Size**: Smaller compiled binaries

## C++ Interoperability

All headers include extern "C" guards for C++ compatibility:

```c
#ifdef __cplusplus
extern "C" {
#endif
// C declarations here
#ifdef __cplusplus
}
#endif
```

This allows the library to be used in C++ projects without issues.

## Compilation Notes

- Requires `.c` and `.h` files (not `.cpp`)
- Compiler must support C99 or later
- All Arduino core functions (pinMode, digitalWrite, etc.) remain compatible
- No additional flags or settings required

## Memory Layout

### Multiplexer Structure
- Size: ~16 bytes (4 int pointers)
- Stack allocated, no dynamic memory

### Sensor Structure
- Size: ~40 bytes (8 ints for buffer + 2 ints for range)
- Stack allocated, no dynamic memory

## Examples

See `main.ino` and `test.ino` for complete working examples using the C API.

## Troubleshooting

**Issue: "multiplexer_init not found" error**
- Ensure multiplexer.c is compiled alongside your sketch
- Check that #include directives are correct

**Issue: "expected pointer but got struct" error**
- Remember functions take pointers: `multiplexer_begin(&mux)`
- Not: `multiplexer_begin(mux)`

**Issue: Compilation errors in C++ projects**
- Verify extern "C" guards are present in headers
- Include headers in this order: multiplexer.h, then sensor.h

## Performance Characteristics

- No virtual function calls (C has no inheritance)
- Direct function calls with predictable overhead
- Inline-friendly code for compiler optimization
- Similar performance to original monolithic implementation
- Slightly better cache locality due to struct organization
