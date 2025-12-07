# ----------------------------------------------------------------------------- 
# Makefile pour le projet TSP
# Compile les fichiers C, crée l'exécutable et génère la documentation
# -----------------------------------------------------------------------------

# Recherche automatique de tous les fichiers source C (.c)
SRC := $(shell find . -type f -name "*.c")

# Liste des fichiers objets correspondants (.o)
OBJ := $(SRC:.c=.o)

# Recherche des fichiers d'en-tête (.h) pour gérer les dépendances
HEADERS := $(shell find . -type f -name "*.h")

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lm

# Nom de l'exécutable final
EXEC = main

# ----------------------------------------------------------------------------- 
# Cibles phonie
# -----------------------------------------------------------------------------
.PHONY: all clean docs

# ----------------------------------------------------------------------------- 
# Cible par défaut : build de l'exécutable + documentation
# -----------------------------------------------------------------------------
all: $(EXEC) docs

# ----------------------------------------------------------------------------- 
# Construction de l'exécutable à partir des fichiers objets
# -----------------------------------------------------------------------------
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# ----------------------------------------------------------------------------- 
# Compilation générique : .c -> .o
# -----------------------------------------------------------------------------
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# ----------------------------------------------------------------------------- 
# Génération de la documentation avec Doxygen
# -----------------------------------------------------------------------------
docs:
	@echo "Génération de la documentation Doxygen..."
	doxygen Doxyfile
	@echo "Documentation générée dans le dossier 'docs/html'."

# ----------------------------------------------------------------------------- 
# Nettoyage : suppression des objets, de l'exécutable et de la doc
# -----------------------------------------------------------------------------
clean:
	rm -f $(OBJ) $(EXEC)
	rm -rf docs/html
