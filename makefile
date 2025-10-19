# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lm

# Nom de l'exécutable
EXE = main

# Détection automatique de tous les fichiers .c du répertoire
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

# Règle principale
$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LDFLAGS)
# Règle générique de compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(EXE) $(OBJS)
