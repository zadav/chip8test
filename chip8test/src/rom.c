/*
 * rom.c
 * Chargement d'une ROM CHIP-8 en mémoire.
 */

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "rom.h"

/*
 * chargerRom(chemin)
 * ------------------
 * Étapes :
 *   1. Ouvrir le fichier en lecture binaire ("rb")
 *   2. Mesurer sa taille (fseek + ftell)
 *   3. Vérifier qu'elle tient dans la mémoire disponible
 *   4. Lire les octets directement dans cpu.memoire[ADRESSE_DEBUT]
 *   5. Fermer le fichier
 */
int chargerRom(const char *chemin)
{
    /* 1. Ouverture en mode binaire */
    FILE *f = fopen(chemin, "rb");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir '%s'\n", chemin);
        return -1;
    }

    /* 2. Mesure de la taille du fichier */
    fseek(f, 0, SEEK_END);   /* aller à la fin */
    long taille = ftell(f);  /* lire la position = taille en octets */
    fseek(f, 0, SEEK_SET);   /* revenir au début pour la lecture */

    /* 3. Vérification : la ROM doit tenir dans la mémoire disponible */
    long max = TAILLE_MEMOIRE - ADRESSE_DEBUT;  /* 3584 octets */
    if (taille > max) {
        fprintf(stderr, "Erreur : ROM trop grande (%ld octets, max %ld)\n",
                taille, max);
        fclose(f);
        return -1;
    }

    /* 4. Lecture dans la mémoire à partir de l'adresse 0x200 */
    size_t lu = fread(&cpu.memoire[ADRESSE_DEBUT], 1, (size_t)taille, f);
    if ((long)lu != taille) {
        fprintf(stderr, "Erreur : lecture incomplète (%zu/%ld octets)\n",
                lu, taille);
        fclose(f);
        return -1;
    }

    /* 5. Fermeture */
    fclose(f);

    printf("ROM chargee : '%s' (%ld octets a partir de 0x%04X)\n",
           chemin, taille, ADRESSE_DEBUT);
    return 0;
}
