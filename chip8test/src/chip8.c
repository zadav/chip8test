/*
 * chip8.c
 * Point d'entrée de l'émulateur CHIP-8.
 *
 * Étape 2 : chargement d'une ROM passée en argument.
 * Usage : ./chip8 <fichier.ch8>
 */

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "rom.h"

int main(int argc, char *argv[])
{
    /* Vérification de l'argument */
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <fichier.ch8>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* --- Initialisation du CPU --- */
    initialiserCpu();

    /* --- Chargement de la ROM --- */
    if (chargerRom(argv[1]) != 0)
        return EXIT_FAILURE;

    /* --- Affichage de l'état initial --- */
    printf("\n=== Etat initial du CPU ===\n");
    printf("PC      : 0x%04X\n", cpu.pc);
    printf("I       : 0x%04X\n", cpu.I);
    printf("V0..VF  : ");
    for (int i = 0; i < 16; i++)
        printf("%02X ", cpu.V[i]);
    printf("\n");

    /* --- Aperçu des 8 premiers octets de la ROM en mémoire --- */
    printf("\nMemoire[0x200..0x207] : ");
    for (int i = 0; i < 8; i++)
        printf("%02X ", cpu.memoire[ADRESSE_DEBUT + i]);
    printf("\n");

    /*
     * Prochain ajout ici :
     *   - boucle principale : fetch / decode / execute
     */

    return EXIT_SUCCESS;
}
