# Makefile robusto para compilar e rodar testes em C

CC = gcc
# Adiciona -DARDUINO=0 para desabilitar include de Arduino.h se definido no código
CFLAGS = -Wall -Wextra -Isrc/mux -Isrc/sensor -I$(PWD) -DARDUINO=0
# Exclui arquivos com problemas ou não C puro
SRC_LIB = $(filter-out src/sensor/test.c, $(wildcard src/mux/*.c src/sensor/*.c))
# Procura testes na raiz e em src/test
TEST_FILES = $(wildcard *test* *.test* src/test/*test* src/test/*.test*)
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
