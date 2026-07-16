// Multiplexer test.ino
// Complete test suite for the mux-sensors-lib library
// Designed to test all functionality on Arduino IDE

// Pin definitions for 74HC4067 multiplexer

const int S2 = 6;
const int SIG = A0;

// Sensor configuration
int raw_channels[8];
int normalized_channels[8];

// Test state
int test_case = 0;
unsigned long last_test_time = 0;
const unsigned long TEST_INTERVAL = 3000; // 3 seconds between tests

// Function declarations
void run_diagnostic_test();
void run_single_channel_test();
void run_all_channels_test();
void run_normalization_test();
void print_sensor_data(int* data, const char* label);
void print_status_message(const char* message);

// Multiplexer setup
void setup_mux() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
}

// Read all 8 channels
void read_mux_all(int* buffer) {
  for (int channel = 0; channel < 8; channel++) {
    // Set binary address on S0, S1, S2
    digitalWrite(S0, (channel & 0x01));           // Bit 0
    digitalWrite(S1, (channel & 0x02) >> 1);      // Bit 1
    digitalWrite(S2, (channel & 0x04) >> 2);      // Bit 2
    
    // Allow time for the multiplexer to settle
    delayMicroseconds(100);
    
    // Read the analog signal
    buffer[channel] = analogRead(SIG);
  }
}

// Read single channel
int read_mux_channel(int channel) {
  if (channel < 0 || channel > 7) return 0;
  
  digitalWrite(S0, (channel & 0x01));
  digitalWrite(S1, (channel & 0x02) >> 1);
  digitalWrite(S2, (channel & 0x04) >> 2);
  
  delayMicroseconds(100);
  return analogRead(SIG);
}

// Normalize data from 0-1023 to 0-255
void normalize_data(int* raw, int* normalized) {
  for (int i = 0; i < 8; i++) {
    normalized[i] = map(raw[i], 0, 1023, 0, 255);
  }
}

// Normalize single value
int normalize_value(int raw) {
  return map(raw, 0, 1023, 0, 255);
}

void setup() {
  Serial.begin(9600);
  
  setup_mux();
  
  print_status_message("=== MUX-SENSORS-LIB TEST SUITE ===");
  print_status_message("Multiplexer pins: S0=4, S1=5, S2=6, SIG=A0");
  print_status_message("Starting diagnostic tests...");
}

void loop() {
  // Run tests sequentially every TEST_INTERVAL milliseconds
  if (millis() - last_test_time >= TEST_INTERVAL) {
    last_test_time = millis();
    
    switch(test_case) {
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
  Serial.print(S0);
  Serial.print(", S1=");
  Serial.print(S1);
  Serial.print(", S2=");
  Serial.println(S2);
  
  Serial.print("Signal pin (SIG): A");
  Serial.println(SIG - A0);
  
  // Test each address pin
  int address_pins[] = {S0, S1, S2};
  for (int pin = 0; pin < 3; pin++) {
    digitalWrite(address_pins[pin], HIGH);
    delay(100);
    int pin_state = digitalRead(address_pins[pin]);
    digitalWrite(address_pins[pin], LOW);
    
    Serial.print("Address pin ");
    Serial.print(pin);
    Serial.print(" (GPIO ");
    Serial.print(address_pins[pin]);
    Serial.print(") state: ");
    Serial.println(pin_state ? "HIGH" : "LOW");
  }
  
  print_status_message("Diagnostic test complete.");
}

// Test 2: Single channel read test
void run_single_channel_test() {
  print_status_message("\n[TEST 2] SINGLE CHANNEL READ");
  
  for (int ch = 0; ch < 8; ch++) {
    int value = read_mux_channel(ch);
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
  read_mux_all(raw_channels);
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
  
  Serial.print("Normalization range: 0 - 255");
  Serial.println();
  
  read_mux_all(raw_channels);
  normalize_data(raw_channels, normalized_channels);
  
  print_sensor_data(raw_channels, "Raw ADC Values (0-1023)");
  print_sensor_data(normalized_channels, "Normalized Values (0-255)");
  
  print_status_message("Normalization test complete.");
}

// Helper: Print sensor data array
void print_sensor_data(int* data, const char* label) {
  Serial.print(">> ");
  Serial.println(label);
  
  for (int i = 0; i < 8; i++) {
    Serial.print("   CH");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(data[i]);
  }
}

// Helper: Print status messages
void print_status_message(const char* message) {
  Serial.println(message);
}
