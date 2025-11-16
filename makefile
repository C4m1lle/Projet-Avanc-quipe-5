# Recherche automatiquement tous les .c dans l'arborescence
SRC := $(shell find . -type f -name "*.c")
OBJ := $(SRC:.c=.o)

# Recherche automatiquement les .h (utile pour les dépendances)
HEADERS := $(shell find . -type f -name "*.h")

CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lm

EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compilation générique
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
