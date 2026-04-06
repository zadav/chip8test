/*
 * cpu.h
 * Structure du CPU CHIP-8 et déclarations des fonctions associées.
 */

#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>

/* Taille totale de la mémoire CHIP-8 : 4096 octets */
#define TAILLE_MEMOIRE  4096

/*
 * Les 512 premiers octets (0x000–0x1FF) sont réservés à l'interpréteur.
 * Les ROMs sont chargées à partir de l'adresse 512 (0x200).
 */
#define ADRESSE_DEBUT   512

/*
 * Structure CPU — représente l'état complet de la machine CHIP-8.
 *
 *  memoire   : 4 Ko de RAM partagée (programme + données + sprites)
 *  V         : 16 registres généraux 8 bits, nommés V0 à VF
 *              VF est souvent utilisé comme flag (retenue, collision...)
 *  I         : registre d'adresse 16 bits (pointe vers la mémoire)
 *  saut      : pile d'appel — stocke les adresses de retour (max 16)
 *  nbrsaut   : sommet de pile (index du prochain slot libre)
 *  compteurJeu : timer de délai, décrémenté à 60 Hz jusqu'à 0
 *  compteurSon : timer sonore, émet un bip tant qu'il est > 0
 *  pc        : Program Counter — adresse de l'instruction courante
 */
typedef struct {
    uint8_t  memoire[TAILLE_MEMOIRE];
    uint8_t  V[16];
    uint16_t I;
    uint16_t saut[16];
    uint8_t  nbrsaut;
    uint8_t  compteurJeu;
    uint8_t  compteurSon;
    uint16_t pc;
} CPU;

/* Instance globale du CPU */
extern CPU cpu;

/* Remet le CPU dans son état initial (registres à 0, pc à ADRESSE_DEBUT) */
void initialiserCpu(void);

/* Décrémente les timers s'ils sont supérieurs à 0 (appelé à 60 Hz) */
void decompter(void);

#endif /* CPU_H_ */
