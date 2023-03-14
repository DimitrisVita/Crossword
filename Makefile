SRC := ./src
INCLUDE := ./include
DATA_DIR := ./data

CC = gcc
CFLAGS = -Wall -Werror -g -I$(INCLUDE) -O2

ARGS ?= $(DATA_DIR)/Crossword1.txt

OUT := crossw
SRC := $(wildcard $(SRC)/*.c)
OBJ := $(patsubst %.c,%.o,$(SRC))

$(OUT): $(OBJ)
	$(CC) $(FLAGS) -o $(OUT) $(OBJ)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

run: $(OUT)
	./$(OUT) $(ARGS)
	@rm -f $(OBJ)

clean:
	rm -f $(OUT) $(OBJ)

.PHONY: build run clean