/*
 * chip8.c
 * Point d'entrée de l'émulateur CHIP-8.
 *
 * Étape 3 : boucle principale fetch / decode / execute.
 * Usage : ./chip8 <fichier.ch8>
 */

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "rom.h"
#include "cpu_exec.h"

/*
 * Nombre maximum de cycles exécutés par la boucle de test.
 * Évite une boucle infinie tant qu'on n'a pas de fenêtre graphique.
 */
#define MAX_CYCLES 20

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <fichier.ch8>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* --- Initialisation --- */
    initialiserCpu();

    /* --- Chargement de la ROM --- */
    if (chargerRom(argv[1]) != 0)
        return EXIT_FAILURE;

    printf("\n=== Debut de l'execution (max %d cycles) ===\n\n", MAX_CYCLES);

    /*
     * ── BOUCLE PRINCIPALE ──────────────────────────────────────────────
     * Chaque itération = un cycle CPU :
     *   1. Afficher l'état courant (PC + opcode qui va être exécuté)
     *   2. Exécuter l'instruction
     *   3. Décrémenter les timers (normalement à 60 Hz ; simplifié ici)
     */
    for (int cycle = 0; cycle < MAX_CYCLES; cycle++) {
        /* Affichage du cycle en cours pour suivre l'exécution */
        uint16_t opcode = (cpu.memoire[cpu.pc] << 8) | cpu.memoire[cpu.pc + 1];
        printf("Cycle %2d | PC=0x%04X | opcode=0x%04X\n",
               cycle, cpu.pc, opcode);

        /* Exécuter l'instruction */
        if (executerInstruction() != 0) {
            fprintf(stderr, "Arret sur erreur.\n");
            return EXIT_FAILURE;
        }

        /* Décrémenter les timers */
        decompter();
    }

    printf("\n=== Fin des %d cycles ===\n", MAX_CYCLES);
    printf("PC final : 0x%04X\n", cpu.pc);

    return EXIT_SUCCESS;
}
