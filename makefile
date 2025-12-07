# -----------------------------------------------------------------------------
# Makefile pour le projet TSP
# Génère automatiquement les fichiers objets et l'exécutable
# -----------------------------------------------------------------------------

# Recherche automatiquement tous les fichiers source C (.c) dans l'arborescence
SRC := $(shell find . -type f -name "*.c")

# Génère la liste des fichiers objets correspondants (.o)
OBJ := $(SRC:.c=.o)

# Recherche automatiquement tous les fichiers d'en-tête (.h) pour gérer les dépendances
HEADERS := $(shell find . -type f -name "*.h")

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c11   # activer tous les warnings et utiliser C11
LDFLAGS = -lm                     # linker avec la librairie mathématique

# Nom de l'exécutable final
EXEC = main

# -----------------------------------------------------------------------------
# Cible par défaut : build l'exécutable
# -----------------------------------------------------------------------------
all: $(EXEC)

# -----------------------------------------------------------------------------
# Construction de l'exécutable à partir des fichiers objets
# $@ = cible, $(OBJ) = liste des objets
# -----------------------------------------------------------------------------
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# -----------------------------------------------------------------------------
# Compilation générique : .c -> .o
# $< = fichier source, $@ = fichier cible
# Dépend des headers pour recompilation si un header change
# -----------------------------------------------------------------------------
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# -----------------------------------------------------------------------------
# Nettoyage : suppression des objets et de l'exécutable
# -----------------------------------------------------------------------------
clean:
	rm -f $(OBJ) $(EXEC)
