# Makefile — CHIP-8 émulateur
#
# Utilisation :
#   make        → compile le projet
#   make clean  → supprime les fichiers compilés
#   make run    → compile puis lance l'émulateur (nécessite un fichier .ch8)

# Compilateur et options de base
CC      = gcc
# -Wall   : active tous les avertissements courants
# -Wextra : avertissements supplémentaires
# -g      : inclut les infos de débogage (pour gdb)
# -std=c11: utilise le standard C11
CFLAGS  = -Wall -Wextra -g -std=c11

# Flags SDL2 (inclus et libs)
# `sdl2-config --cflags` donne  : -I/usr/include/SDL2 -D_REENTRANT
# `sdl2-config --libs`   donne  : -lSDL2
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS   = $(shell sdl2-config --libs)

# Dossier des sources
SRC_DIR = chip8test/src

# Liste des fichiers sources (.c)
SRCS    = $(SRC_DIR)/chip8.c \
          $(SRC_DIR)/cpu.c \
          $(SRC_DIR)/rom.c \
          $(SRC_DIR)/cpu_exec.c \
          $(SRC_DIR)/ecran.c \
          $(SRC_DIR)/clavier.c

# Nom de l'exécutable final
TARGET  = chip8

# Règle par défaut : construire l'exécutable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -o $(TARGET) $(SRCS) $(SDL_LIBS)

# Lance l'émulateur (passe la ROM en argument)
run: $(TARGET)
	./$(TARGET) $(ROM)

# Supprime l'exécutable
clean:
	rm -f $(TARGET)

# "Phony" = ces cibles ne correspondent pas à des fichiers réels
.PHONY: run clean
