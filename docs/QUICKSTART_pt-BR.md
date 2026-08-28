# Guia Rápido (Quick Start) - mux-sensors-lib

## Setup em 5 Minutos

### 1. Conexões de Hardware

Conecte a sua placa Arduino no multiplexador CD74HC4067 da seguinte forma:

```
Arduino Pino 4  ──→  S0 (Linha de Endereço 0)
Arduino Pino 5  ──→  S1 (Linha de Endereço 1)
Arduino Pino 6  ──→  S2 (Linha de Endereço 2)
Arduino Pino A0 ──→  SIG (Sinal de Saída)
Arduino GND     ──→  GND (Terra do Multiplexador)
Arduino 5V      ──→  VCC (Energia do Multiplexador)

Sensor 0 ──→  Canal 0 do Multiplexador
Sensor 1 ──→  Canal 1 do Multiplexador
... (até 8 sensores)
```

### 2. Testando a Biblioteca

1. Abra a Arduino IDE.
2. Instale a biblioteca acessando: **Sketch** > **Incluir Biblioteca** > **Gerenciar Bibliotecas...** > e pesquise por "mux-sensors-lib".
3. Abra o exemplo oficial: **Arquivo** > **Exemplos** > **mux-sensors-lib** > **DiagnosticTest**.
4. Selecione a sua placa: **Ferramentas** > **Placa**.
5. Selecione a porta conectada: **Ferramentas** > **Porta**.
6. Clique no botão de Carregar (Upload).
7. Abra o Monitor Serial: **Ferramentas** > **Monitor Serial** (Certifique-se de que a taxa de baud está em **9600**).
8. Observe a saída. Você verá informações de diagnóstico e os valores dos sensores sendo lidos!

### 3. Usando no seu próprio projeto

Crie um novo sketch vazio no Arduino IDE e cole o código abaixo.

> **O que esse código faz?**
> Ele inicializa os pinos de controle e lê todos os 8 canais de uma vez, convertendo o valor lido do Arduino (que vai de 0 a 1023) para um valor de 0 a 255.

```c
#include <mux_sensors_lib.h>

Multiplexer mux;       // Estrutura que guarda os pinos do multiplexador
Sensor sensor;         // Estrutura que guarda a faixa de valores para conversão

int raw_values[8];         // Vetor para guardar a leitura original (0-1023)
int normalized_values[8];  // Vetor para guardar a leitura convertida

void setup() {
  Serial.begin(9600);
  
  // Cria o multiplexador: pinos S0=4, S1=5, S2=6, Sinal=A0
  mux = multiplexer_init(4, 5, 6, A0);
  
  // Cria o sensor dizendo que queremos converter para a faixa de 0 a 255
  sensor = sensor_init(0, 255);
  
  // Configura os pinos do Arduino como saída e entrada
  multiplexer_begin(&mux);
}

void loop() {
  // Lê todos os 8 canais e guarda no vetor raw_values
  multiplexer_read_all(&mux, raw_values);
  
  // Converte a leitura original para a faixa de 0 a 255
  sensor_normalize_buffer(&sensor, raw_values, normalized_values);
  
  // Imprime o valor de cada canal na tela
  for (int i = 0; i < 8; i++) {
    Serial.print("Canal ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(normalized_values[i]);
  }
  
  delay(100);
}
```

### Configurações Comuns

**Se quiser usar pinos diferentes:**
```c
mux = multiplexer_init(10, 11, 12, A1);  // Pinos S0, S1, S2 e Sinal diferentes
```

**Se quiser mudar a faixa de valores (Range):**
```c
Sensor sensor = sensor_init(-100, 100); // Converte os valores para irem de -100 a +100
sensor_set_range(&sensor, 0, 1023);     // Ou mude mais tarde usando essa função
```

**Se quiser ler apenas UM canal, ao invés dos 8:**
```c
int valor = multiplexer_read_channel(&mux, 3);  // Lê apenas o Canal 3
```

## Solução de Problemas (Troubleshooting)

**O Monitor Serial não mostra nada:**
- Verifique se o Baud Rate está configurado para 9600 na parte de baixo do Monitor Serial.
- Reinicie a Arduino IDE.

**Todos os canais mostram o valor 0:**
- Verifique se os cabos 5V e GND do multiplexador estão bem conectados no Arduino.
- Verifique se o pino SIG (Sinal) está conectado no A0.

**O upload falha:**
- Verifique se você selecionou a Placa (Board) e a Porta (Port) corretas no menu Ferramentas.

## Precisa de Ajuda?

- Quer entender as funções detalhadamente? Veja a seção API Reference no arquivo [README.md](../README.md).
- Quer ajudar a programar a biblioteca? Leia o nosso [CONTRIBUTING.md](../CONTRIBUTING.md).
