/*
 * cpu.c
 * Implémentation des fonctions de base du CPU CHIP-8.
 */

#include <string.h>   /* memset */
#include "cpu.h"

/* Instance globale du CPU (déclarée extern dans cpu.h) */
CPU cpu;

/*
 * Sprites de fonte : représentation des chiffres hexadécimaux 0–F.
 * Chaque chiffre = 5 octets = 5 lignes de 8 pixels (seuls les 4 bits
 * de gauche sont utilisés, soit une largeur effective de 4 pixels).
 *
 * Exemple pour '0' :
 *   0xF0 = 1111 0000  → ████
 *   0x90 = 1001 0000  → █  █
 *   0x90 = 1001 0000  → █  █
 *   0x90 = 1001 0000  → █  █
 *   0xF0 = 1111 0000  → ████
 */
static const uint8_t fonte[16 * 5] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
    0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
    0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
    0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
    0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
    0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
    0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
    0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
    0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
    0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
    0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
    0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
    0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
    0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
    0xF0, 0x80, 0xF0, 0x80, 0x80, /* F */
};

/*
 * initialiserCpu()
 * ----------------
 * Remet tous les champs du CPU à zéro, positionne le PC à ADRESSE_DEBUT,
 * et charge les sprites de fonte aux adresses 0x000–0x04F.
 */
void initialiserCpu(void)
{
    memset(&cpu, 0, sizeof(CPU));
    cpu.pc = ADRESSE_DEBUT;

    /* Copier la fonte dans les 80 premiers octets de mémoire */
    memcpy(cpu.memoire, fonte, sizeof(fonte));
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
