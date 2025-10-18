# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Nom de l'exécutable
EXE = main

# Détection automatique de tous les fichiers .c du répertoire
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

# Règle principale
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXE)

# Règle générique de compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(EXE) $(OBJS)
