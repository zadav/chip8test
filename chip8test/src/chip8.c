/*
 * chip8.c
 * Point d'entrée de l'émulateur CHIP-8.
 *
 * Étape 1 : initialisation du CPU et affichage de l'état de base.
 * Les étapes suivantes ajouteront le chargement de ROM et la boucle d'exécution.
 */

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

int main(void)
{
    /* --- Initialisation --- */
    initialiserCpu();

    /* --- Vérification : afficher l'état initial du CPU --- */
    printf("=== CHIP-8 initialise ===\n");
    printf("PC      : 0x%04X  (attendu : 0x%04X)\n", cpu.pc, ADRESSE_DEBUT);
    printf("I       : 0x%04X\n", cpu.I);
    printf("V0..VF  : ");
    for (int i = 0; i < 16; i++)
        printf("%02X ", cpu.V[i]);
    printf("\n");
    printf("Pile    : vide (%d niveaux max)\n", 16);
    printf("Timers  : jeu=%d  son=%d\n", cpu.compteurJeu, cpu.compteurSon);

    /*
     * Prochain ajout ici :
     *   - charger une ROM en mémoire à partir de cpu.memoire[ADRESSE_DEBUT]
     *   - boucle fetch / decode / execute
     */

    return EXIT_SUCCESS;
}
