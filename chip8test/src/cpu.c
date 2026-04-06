/*
 * cpu.c
 * Implémentation des fonctions de base du CPU CHIP-8.
 */

#include <string.h>   /* memset */
#include "cpu.h"

/* Instance globale du CPU (déclarée extern dans cpu.h) */
CPU cpu;

/*
 * initialiserCpu()
 * ----------------
 * Remet tous les champs du CPU à zéro, puis positionne le PC à ADRESSE_DEBUT.
 *
 * Pourquoi 0x200 (512) ?
 *   Sur les machines originales, les 512 premiers octets contenaient
 *   l'interpréteur CHIP-8 lui-même. Les ROMs commencent donc toujours
 *   à l'adresse 0x200.
 */
void initialiserCpu(void)
{
    memset(&cpu, 0, sizeof(CPU));
    cpu.pc = ADRESSE_DEBUT;
}

/*
 * decompter()
 * -----------
 * Décrémente les deux timers si leur valeur est supérieure à 0.
 * Cette fonction doit être appelée 60 fois par seconde (60 Hz).
 *
 *  compteurJeu : timer de délai général (utilisé par certains jeux
 *                pour synchroniser la vitesse)
 *  compteurSon : tant qu'il est > 0, le buzzer émet un bip
 */
void decompter(void)
{
    if (cpu.compteurJeu > 0)
        cpu.compteurJeu--;

    if (cpu.compteurSon > 0)
        cpu.compteurSon--;
}
