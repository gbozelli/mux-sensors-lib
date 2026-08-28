/*
 * Mock mínimo do Arduino.h para permitir compilar e rodar os testes
 * unitários em C puro (gcc), fora do Arduino IDE.
 *
 * Este arquivo NÃO substitui o Arduino.h real: ele só define o
 * suficiente (constantes, tipos e funções) para o código em
 * src/mux e src/sensor compilar e linkar nativamente durante
 * `make test`. Quando compilado de verdade para uma placa
 * (ARDUINO > 0), o Arduino.h real do core é usado no lugar deste.
 */
#ifndef ARDUINO_MOCK_H
#define ARDUINO_MOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Constantes básicas ---- */
#define HIGH 0x1
#define LOW 0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

/* Pinos analógicos comuns (o suficiente para os testes atuais) */
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

/* ---- Funções de I/O digital/analógico (stubs) ----
 * `static inline` para poder ser incluído em vários arquivos .c
 * que são linkados juntos (evita erro de símbolo duplicado).
 */
static inline void pinMode(int pin, int mode) {
    (void)pin;
    (void)mode;
}

static inline void digitalWrite(int pin, int value) {
    (void)pin;
    (void)value;
}

static inline int digitalRead(int pin) {
    (void)pin;
    return LOW;
}

/* Valor fixo "neutro" (meio da escala 0-1023) para os testes
 * automatizados não dependerem de hardware real. */
static inline int analogRead(int pin) {
    (void)pin;
    return 512;
}

static inline void analogWrite(int pin, int value) {
    (void)pin;
    (void)value;
}

/* ---- Tempo (stubs) ---- */
static inline void delay(unsigned long ms) { (void)ms; }

static inline void delayMicroseconds(unsigned int us) { (void)us; }

static inline unsigned long millis(void) { return 0; }

static inline unsigned long micros(void) { return 0; }

/* ---- Matemática (igual à implementação real do core Arduino) ---- */
static inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#ifdef __cplusplus
}
#endif

#endif /* ARDUINO_MOCK_H */
