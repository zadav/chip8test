/*
 * chip8.c
 * Point d'entrée de l'émulateur CHIP-8.
 *
 * Étape 4 : boucle principale avec affichage SDL2.
 * Usage : ./chip8 <fichier.ch8>
 *
 * Architecture de la boucle :
 *   Le CHIP-8 tourne typiquement à ~500 Hz (500 instructions/sec).
 *   Les timers et l'affichage se mettent à jour à 60 Hz.
 *   → On exécute environ 500/60 ≈ 8 instructions par frame.
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "cpu.h"
#include "rom.h"
#include "cpu_exec.h"
#include "ecran.h"

/* Nombre d'instructions CPU exécutées par frame (60 Hz) */
#define CYCLES_PAR_FRAME  8

/* Durée d'une frame en millisecondes : 1000 ms / 60 ≈ 16 ms */
#define MS_PAR_FRAME      16

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <fichier.ch8>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* --- Initialisation du CPU et chargement de la ROM --- */
    initialiserCpu();
    if (chargerRom(argv[1]) != 0)
        return EXIT_FAILURE;

    /* --- Ouverture de la fenêtre SDL2 --- */
    if (ecranOuvrir() != 0)
        return EXIT_FAILURE;

    printf("Emulateur demarre. Fermer la fenetre pour quitter.\n");

    /*
     * ── BOUCLE PRINCIPALE ──────────────────────────────────────────────
     *
     *  Chaque itération = une frame (≈16 ms) :
     *
     *  1. Traiter les événements SDL (fermeture de fenêtre, clavier…)
     *  2. Exécuter CYCLES_PAR_FRAME instructions CPU
     *  3. Décrémenter les timers (à 60 Hz)
     *  4. Rafraîchir l'affichage
     *  5. Attendre le reste du temps pour rester à 60 Hz
     */
    int actif = 1;
    while (actif) {

        uint32_t debut = SDL_GetTicks();

        /* 1. Événements SDL */
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                actif = 0;
        }

        /* 2. Exécuter les instructions CPU de cette frame */
        for (int i = 0; i < CYCLES_PAR_FRAME && actif; i++) {
            if (executerInstruction() != 0) {
                fprintf(stderr, "Arret sur erreur CPU.\n");
                actif = 0;
            }
        }

        /* 3. Timers à 60 Hz */
        decompter();

        /* 4. Affichage */
        ecranAfficher();

        /* 5. Limiter à ~60 Hz */
        uint32_t elapsed = SDL_GetTicks() - debut;
        if (elapsed < MS_PAR_FRAME)
            SDL_Delay(MS_PAR_FRAME - elapsed);
    }

    /* --- Nettoyage --- */
    ecranFermer();
    return EXIT_SUCCESS;
}
