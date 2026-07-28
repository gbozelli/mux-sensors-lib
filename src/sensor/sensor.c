#include "sensor.h"

// Inicializa o sensor com valores mínimos e máximos.
Sensor sensor_init(int min_val, int max_val) {
  Sensor sensor;
  sensor.min_value = min_val;
  sensor.max_value = max_val;
  // Inicializa o buffer de valores do sensor com zeros para evitar lixo de memória.
  for (int i = 0; i < SENSOR_NUM_SENSORS; i++) {
    sensor.buffer[i] = 0;
  }
  return sensor;
}

// Define os valores mínimo e máximo do sensor através de ponteiros para permitir
// a modificação dos valores fora da função de inicialização.
void sensor_set_range(Sensor* sensor, int min_val, int max_val) {
  sensor->min_value = min_val;
  sensor->max_value = max_val;
}

// Normaliza um valor bruto do sensor para a escala definida pelos valores mínimo 
// e máximo.
int sensor_normalize(Sensor* sensor, int raw_value) {
  // A função map() executa uma interpolação linear para converter o valor bruto do sensor (raw_value)
  // para um valor normalizado na faixa definida.
  return map(raw_value, 0, 1023, sensor->min_value, sensor->max_value);
}

// Normaliza todos os valores brutos do sensor e armazena os resultados em um buffer separado.
void sensor_normalize_buffer(Sensor* sensor, int* raw_buffer, int* normalized_buffer) {
  // Itera sobre todos os valores brutos do sensor, normalizando cada um e armazenando o resultado
  for (int i = 0; i < SENSOR_NUM_SENSORS; i++) {
    normalized_buffer[i] = sensor_normalize(sensor, raw_buffer[i]);
  }
}

// Retorna o valor armazenado no buffer do sensor para um índice específico.
int sensor_get_value(Sensor* sensor, int index) {
  // Verifica se o índice está dentro do intervalo válido antes de acessar o buffer.
  if (index >= 0 && index < SENSOR_NUM_SENSORS) {
    return sensor->buffer[index];
  }
  // Retorna -1 para indicar que o índice está fora do intervalo válido.
  return -1;
}

// Armazena um valor no buffer na posição do índice especificado.
void sensor_set_value(Sensor* sensor, int index, int value) {
  // Verifica se o índice está dentro do intervalo válido antes de modificar o buffer.
  if (index >= 0 && index < SENSOR_NUM_SENSORS) {
    sensor->buffer[index] = value;
  }
}
