// Pin definitions for 74HC4067 multiplexer
const int S0 = A1;
const int S1 = A2;
const int S2 = A3;
const int SIG = A0;

float buffer[8];

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(SIG, INPUT);

  digitalWrite(S0, LOW); // Somente uma boa prática para garantir que os pinos de endereço comecem em um estado conhecido
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
}

void read_mux_all(int* buffer[8]) { // * é um ponteiro para passar os valores verdadeiros do buffer e não uma cópia
  for (int channel = 0; channel < 8; channel++) {
    // Pega o valor e passa para binário
    digitalWrite(S0, (channel & 0x01));           // Bit 0  
    digitalWrite(S1, (channel & 0x02) >> 1);      // Bit 1
    digitalWrite(S2, (channel & 0x04) >> 2);      // Bit 2
    
    // Allow time for the multiplexer to settle
    // delayMicroseconds(100);
    
    // Read the analog signal
    buffer[channel] = analogRead(SIG); // Armazena os valores pra não ter que declarar 8 vezes, é a fila
  }
}

void print_sensor_data(float* buffer[8]) { // Sempre colocando ponteiros; declara de novo para economizar memória não precisa de 8 variáveis
  Serial.print(">> "); 
  
  for (int i = 0; i < 8; i++) {
    Serial.print("   CH");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(buffer[i]); 
  }
}

void loop() {
  read_mux_all(buffer);
  print_sensor_data(buffer);
}