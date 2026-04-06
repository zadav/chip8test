# Makefile — CHIP-8 émulateur
#
# Utilisation :
#   make        → compile le projet
#   make clean  → supprime les fichiers compilés
#   make run    → compile puis lance l'émulateur

# Compilateur et options
CC      = gcc
# -Wall   : active tous les avertissements courants
# -Wextra : avertissements supplémentaires
# -g      : inclut les infos de débogage (pour gdb)
# -std=c11: utilise le standard C11
CFLAGS  = -Wall -Wextra -g -std=c11

# Dossier des sources
SRC_DIR = chip8test/src

# Liste des fichiers sources (.c)
SRCS    = $(SRC_DIR)/chip8.c \
          $(SRC_DIR)/cpu.c \
          $(SRC_DIR)/rom.c \
          $(SRC_DIR)/cpu_exec.c

# Nom de l'exécutable final
TARGET  = chip8

# Règle par défaut : construire l'exécutable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Lance l'émulateur après compilation
run: $(TARGET)
	./$(TARGET)

# Supprime l'exécutable
clean:
	rm -f $(TARGET)

# "Phony" = ces cibles ne correspondent pas à des fichiers réels
.PHONY: run clean
