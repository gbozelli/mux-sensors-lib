# Makefile robusto para compilar e rodar testes em C

CC = gcc
# Adiciona -DARDUINO=0 para desabilitar include de Arduino.h se definido no código
# -Itest/mocks: onde mora o mock nativo de Arduino.h usado pelo build de testes
CFLAGS = -Wall -Wextra -Isrc/mux -Isrc/sensor -Itest/mocks -I$(PWD) -DARDUINO=0
# Exclui arquivos com problemas ou não C puro
SRC_LIB = $(filter-out src/sensor/test.c, $(wildcard src/mux/*.c src/sensor/*.c))
# Procura testes só em src/test (arquivos .c puros).
# OBS: propositalmente NÃO usa "*test*" na raiz, pois isso pegava
# test.ino (sketch real do Arduino, não compilável com gcc nativo).
TEST_FILES = $(wildcard src/test/*.c)
TEST_BINARIES = $(TEST_FILES:=.out)

all: $(TEST_BINARIES)

%.c.out: %.c $(SRC_LIB)
	$(CC) $(CFLAGS) $^ -o $@

%.test.out: %.test $(SRC_LIB)
	$(CC) $(CFLAGS) $^ -o $@

test: all
	@for bin in $(TEST_BINARIES); do \
	    if [ -x $$bin ]; then ./$$bin; fi \
	done

clean:
	rm -f *.out

.PHONY: all test clean
