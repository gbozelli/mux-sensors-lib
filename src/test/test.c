/*
 * test.c - Arquivo de testes para a mux-sensors-lib
 *
 * Este arquivo roda no computador (com gcc), sem precisar de Arduino.
 * Ele usa o Arduino.h falso (mock) que fica em test/mocks/.
 *
 * Como compilar e rodar:
 *   make test
 *
 * Como funciona:
 *   1. Cada funcao test_xxx() testa UMA coisa da biblioteca
 *   2. Dentro de cada teste, use checar_igual() para comparar
 *      o valor esperado com o valor obtido
 *   3. No final, main() mostra quantos testes passaram
 *
 * Sua tarefa: implementar os testes marcados com TODO!
 */

#include <stdio.h>

#include "multiplexer.h"
#include "sensor.h"

/* Contadores globais de testes */
int testes_ok = 0;
int testes_falhou = 0;

/* ----------------------------------------------------------------
 * Funcao auxiliar para os testes
 * ----------------------------------------------------------------
 *
 * Compara dois numeros inteiros.
 * Se forem iguais    -> conta como teste OK
 * Se forem diferentes -> printa erro e conta como FALHOU
 *
 * Exemplo de uso:
 *   checar_igual(4, mux.pin_s0, "pin_s0 deveria ser 4");
 */
void checar_igual(int esperado, int obtido, const char* descricao) {
    if (esperado == obtido) {
        testes_ok++;
    } else {
        testes_falhou++;
        printf("  FALHOU: %s\n", descricao);
        printf("    Esperado: %d\n", esperado);
        printf("    Obtido:   %d\n", obtido);
    }
}

/* ----------------------------------------------------------------
 * Testes do modulo Multiplexer
 * ---------------------------------------------------------------- */

/*
 * TODO: Teste se multiplexer_init() salva os pinos corretamente.
 *
 * Dica: crie um Multiplexer com multiplexer_init(4, 5, 6, A0)
 * e use checar_igual() para verificar cada campo da struct
 * (mux.pin_s0, mux.pin_s1, mux.pin_s2, mux.pin_sig).
 */
void test_multiplexer_init(void) {
    printf("[TESTE] multiplexer_init\n");

    /* Seu codigo aqui */
}

/* ----------------------------------------------------------------
 * Testes do modulo Sensor
 * ---------------------------------------------------------------- */

/*
 * TODO: Teste se sensor_init() salva min_value e max_value.
 *
 * Dica: crie um Sensor com sensor_init(0, 255)
 * e use checar_igual() para verificar sensor.min_value e sensor.max_value.
 */
void test_sensor_init(void) {
    printf("[TESTE] sensor_init\n");

    /* Seu codigo aqui */
}

/*
 * TODO: Teste se sensor_set_range() muda o range do sensor.
 *
 * Dica: crie um sensor com sensor_init(0, 255),
 * depois chame sensor_set_range(&sensor, 10, 100)
 * e verifique se os valores mudaram.
 */
void test_sensor_set_range(void) {
    printf("[TESTE] sensor_set_range\n");

    /* Seu codigo aqui */
}

/* ----------------------------------------------------------------
 * Funcao principal - roda todos os testes
 * ---------------------------------------------------------------- */

int main(void) {
    printf("=== Testes da mux-sensors-lib ===\n\n");

    /* Chame cada funcao de teste aqui */
    test_multiplexer_init();
    test_sensor_init();
    test_sensor_set_range();

    /* Resultado final */
    printf("\n--- Resultado: %d passaram, %d falharam ---\n", testes_ok, testes_falhou);

    /*
     * Retorna 0 se todos passaram (sucesso para o terminal)
     * Retorna 1 se algum falhou (erro para o terminal / CI)
     */
    if (testes_falhou > 0) {
        return 1;
    }
    return 0;
}
