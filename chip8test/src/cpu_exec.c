/*
 * cpu_exec.c
 * Cycle fetch / decode / execute du CPU CHIP-8.
 *
 * Les opcodes CHIP-8 font toujours 2 octets (big-endian).
 * On les décompose en nibbles pour les décoder :
 *
 *   opcode = 0xABCD
 *              A    → type d'instruction (nibble de poids fort)
 *              B    → registre X  (cpu.V[X])
 *              C    → registre Y  (cpu.V[Y])
 *              D    → nibble de poids faible (N)
 *             BC    → valeur 8 bits (NN)
 *            BCD    → adresse 12 bits (NNN)
 */

#include <stdio.h>
#include "cpu.h"
#include "cpu_exec.h"
#include "ecran.h"
#include "clavier.h"

int executerInstruction(void)
{
    /*
     * ── FETCH ──────────────────────────────────────────────────────────
     * Lire 2 octets consécutifs en mémoire et les assembler en un mot
     * 16 bits. CHIP-8 est big-endian : le premier octet est le plus fort.
     *
     *   memoire[pc]     = 0x12   → octet de poids fort
     *   memoire[pc + 1] = 0x00   → octet de poids faible
     *   opcode          = 0x1200
     */
    uint16_t opcode = (cpu.memoire[cpu.pc] << 8) | cpu.memoire[cpu.pc + 1];

    /*
     * ── DÉCOMPOSITION ──────────────────────────────────────────────────
     * On extrait les différentes parties de l'opcode avec des masques
     * binaires et des décalages.
     */
    uint8_t  X   = (opcode & 0x0F00) >> 8;  /* index du registre VX */
    uint8_t  Y   = (opcode & 0x00F0) >> 4;  /* index du registre VY */
    uint8_t  N   = (opcode & 0x000F);        /* nibble faible */
    uint8_t  NN  = (opcode & 0x00FF);        /* octet bas */
    uint16_t NNN = (opcode & 0x0FFF);        /* adresse 12 bits */

    /* Avancer le PC AVANT d'exécuter (sauf pour les sauts) */
    cpu.pc += 2;

    /*
     * ── DECODE & EXECUTE ───────────────────────────────────────────────
     * On examine le nibble de poids fort (bits 15-12) pour choisir
     * la famille d'instructions, puis on affine si nécessaire.
     */
    switch (opcode & 0xF000)
    {
        /*
         * 0x00E0 — CLS : effacer l'écran
         * 0x00EE — RET : retour de sous-routine
         * (les autres opcodes 0x0NNN sont ignorés sur CHIP-48)
         */
        case 0x0000:
            if (opcode == 0x00E0) {
                /* CLS — efface le buffer de pixels */
                ecranEffacer();
            } else if (opcode == 0x00EE) {
                /* RET — dépiler l'adresse de retour */
                if (cpu.nbrsaut == 0) {
                    fprintf(stderr, "Erreur RET : pile vide\n");
                    return -1;
                }
                cpu.nbrsaut--;
                cpu.pc = cpu.saut[cpu.nbrsaut];
            }
            break;

        /*
         * 0x1NNN — JP NNN : sauter à l'adresse NNN
         * Le PC prend directement la valeur NNN.
         */
        case 0x1000:
            cpu.pc = NNN;
            break;

        /*
         * 0x2NNN — CALL NNN : appeler la sous-routine à NNN
         * On empile le PC actuel (déjà incrémenté) puis on saute.
         */
        case 0x2000:
            if (cpu.nbrsaut >= 16) {
                fprintf(stderr, "Erreur CALL : pile pleine\n");
                return -1;
            }
            cpu.saut[cpu.nbrsaut] = cpu.pc;
            cpu.nbrsaut++;
            cpu.pc = NNN;
            break;

        /*
         * 0x3XNN — SE VX, NN : passer l'instruction suivante si VX == NN
         * "SE" = Skip if Equal
         */
        case 0x3000:
            if (cpu.V[X] == NN)
                cpu.pc += 2;
            break;

        /*
         * 0x4XNN — SNE VX, NN : passer si VX != NN
         * "SNE" = Skip if Not Equal
         */
        case 0x4000:
            if (cpu.V[X] != NN)
                cpu.pc += 2;
            break;

        /*
         * 0x5XY0 — SE VX, VY : passer si VX == VY
         */
        case 0x5000:
            if (cpu.V[X] == cpu.V[Y])
                cpu.pc += 2;
            break;

        /*
         * 0x6XNN — LD VX, NN : charger la valeur NN dans VX
         */
        case 0x6000:
            cpu.V[X] = NN;
            break;

        /*
         * 0x7XNN — ADD VX, NN : ajouter NN à VX (sans flag de retenue)
         */
        case 0x7000:
            cpu.V[X] += NN;
            break;

        /*
         * 0x8XY? — opérations arithmétiques et logiques entre VX et VY
         */
        case 0x8000:
            switch (N) {
                case 0x0: cpu.V[X]  = cpu.V[Y]; break;           /* LD  VX, VY   */
                case 0x1: cpu.V[X] |= cpu.V[Y]; break;           /* OR  VX, VY   */
                case 0x2: cpu.V[X] &= cpu.V[Y]; break;           /* AND VX, VY   */
                case 0x3: cpu.V[X] ^= cpu.V[Y]; break;           /* XOR VX, VY   */
                case 0x4: {                                        /* ADD VX, VY (avec retenue) */
                    uint16_t res = cpu.V[X] + cpu.V[Y];
                    cpu.V[0xF] = (res > 0xFF) ? 1 : 0;
                    cpu.V[X] = (uint8_t)res;
                    break;
                }
                case 0x5: {                                        /* SUB VX, VY */
                    cpu.V[0xF] = (cpu.V[X] >= cpu.V[Y]) ? 1 : 0;
                    cpu.V[X] -= cpu.V[Y];
                    break;
                }
                case 0x6: {                                        /* SHR VX */
                    cpu.V[0xF] = cpu.V[X] & 0x1;
                    cpu.V[X] >>= 1;
                    break;
                }
                case 0x7: {                                        /* SUBN VX, VY */
                    cpu.V[0xF] = (cpu.V[Y] >= cpu.V[X]) ? 1 : 0;
                    cpu.V[X] = cpu.V[Y] - cpu.V[X];
                    break;
                }
                case 0xE: {                                        /* SHL VX */
                    cpu.V[0xF] = (cpu.V[X] >> 7) & 0x1;
                    cpu.V[X] <<= 1;
                    break;
                }
                default:
                    fprintf(stderr, "Opcode inconnu : 0x%04X a 0x%04X\n",
                            opcode, cpu.pc - 2);
                    return -1;
            }
            break;

        /*
         * 0x9XY0 — SNE VX, VY : passer si VX != VY
         */
        case 0x9000:
            if (cpu.V[X] != cpu.V[Y])
                cpu.pc += 2;
            break;

        /*
         * 0xANNN — LD I, NNN : charger NNN dans le registre I
         */
        case 0xA000:
            cpu.I = NNN;
            break;

        /*
         * 0xBNNN — JP V0, NNN : sauter à NNN + V0
         */
        case 0xB000:
            cpu.pc = NNN + cpu.V[0];
            break;

        /*
         * 0xCXNN — RND VX, NN : VX = nombre aléatoire & NN
         * (rand() suffit ici ; on verra une meilleure graine plus tard)
         */
        case 0xC000: {
            extern int rand(void);
            cpu.V[X] = (uint8_t)(rand() & NN);
            break;
        }

        /*
         * 0xDXYN — DRW VX, VY, N : dessiner un sprite
         * ─────────────────────────────────────────────
         * Le sprite fait N lignes de 8 pixels de large.
         * Il est stocké en mémoire à l'adresse cpu.I.
         * Chaque octet = une ligne : le bit 7 est le pixel le plus à gauche.
         *
         * Règle XOR : chaque pixel est XORé avec le pixel existant.
         *   Si un pixel passe de 1 → 0 (collision), on met VF à 1.
         *   Sinon VF = 0.
         *
         * Les coordonnées sont "wrappées" modulo la taille de l'écran.
         */
        case 0xD000: {
            uint8_t px = cpu.V[X] % ECRAN_LARGEUR;
            uint8_t py = cpu.V[Y] % ECRAN_HAUTEUR;
            cpu.V[0xF] = 0;

            for (uint8_t ligne = 0; ligne < N; ligne++) {
                uint8_t octet = cpu.memoire[cpu.I + ligne];
                for (uint8_t col = 0; col < 8; col++) {
                    /* Bit courant : on teste chaque bit de l'octet */
                    if (octet & (0x80 >> col)) {
                        uint8_t ex = (px + col) % ECRAN_LARGEUR;
                        uint8_t ey = (py + ligne) % ECRAN_HAUTEUR;
                        /* Collision : pixel déjà allumé va s'éteindre */
                        if (pixels[ey][ex] == 1)
                            cpu.V[0xF] = 1;
                        pixels[ey][ex] ^= 1;
                    }
                }
            }
            break;
        }

        /*
         * 0xEX9E — SKP VX  : passer l'instruction suivante si touches[VX] == 1
         * 0xEXA1 — SKNP VX : passer si touches[VX] == 0
         */
        case 0xE000:
            if (NN == 0x9E) {
                if (touches[cpu.V[X]])
                    cpu.pc += 2;
            } else if (NN == 0xA1) {
                if (!touches[cpu.V[X]])
                    cpu.pc += 2;
            } else {
                fprintf(stderr, "Opcode inconnu : 0x%04X a 0x%04X\n",
                        opcode, cpu.pc - 2);
                return -1;
            }
            break;

        /*
         * 0xFX?? — opérations diverses (timers, mémoire, BCD...)
         */
        case 0xF000:
            switch (NN) {
                case 0x0A: {
                    /*
                     * LD VX, K — attendre une touche, stocker son index dans VX.
                     * Technique : on recule le PC de 2 pour ré-exécuter cet opcode
                     * au prochain cycle jusqu'à ce qu'une touche soit pressée.
                     * Dès qu'une touche est détectée, on la stocke dans VX et on
                     * avance normalement.
                     */
                    int trouvee = -1;
                    for (int k = 0; k < 16; k++) {
                        if (touches[k]) { trouvee = k; break; }
                    }
                    if (trouvee < 0)
                        cpu.pc -= 2;  /* bloquer : ré-exécuter FX0A */
                    else
                        cpu.V[X] = (uint8_t)trouvee;
                    break;
                }
                case 0x07: cpu.V[X] = cpu.compteurJeu;  break;  /* LD VX, DT */
                case 0x15: cpu.compteurJeu = cpu.V[X];  break;  /* LD DT, VX */
                case 0x18: cpu.compteurSon = cpu.V[X];  break;  /* LD ST, VX */
                case 0x1E: cpu.I += cpu.V[X];           break;  /* ADD I, VX */
                case 0x29:
                    /*
                     * LD F, VX — I pointe vers le sprite du chiffre VX (0–F).
                     * Les 16 sprites de fonte sont chargés en mémoire par
                     * initialiserCpu() à l'adresse 0x000, 5 octets chacun.
                     * Donc sprite de VX = adresse 0x000 + VX * 5.
                     */
                    cpu.I = cpu.V[X] * 5;
                    break;
                case 0x33: {
                    /* LD B, VX — stocker VX en BCD à I, I+1, I+2
                     * BCD = représentation décimale chiffre par chiffre */
                    cpu.memoire[cpu.I]     = cpu.V[X] / 100;
                    cpu.memoire[cpu.I + 1] = (cpu.V[X] / 10) % 10;
                    cpu.memoire[cpu.I + 2] = cpu.V[X] % 10;
                    break;
                }
                case 0x55: {
                    /* LD [I], VX — sauvegarder V0..VX en mémoire */
                    for (uint8_t i = 0; i <= X; i++)
                        cpu.memoire[cpu.I + i] = cpu.V[i];
                    break;
                }
                case 0x65: {
                    /* LD VX, [I] — charger V0..VX depuis la mémoire */
                    for (uint8_t i = 0; i <= X; i++)
                        cpu.V[i] = cpu.memoire[cpu.I + i];
                    break;
                }
                default:
                    fprintf(stderr, "Opcode inconnu : 0x%04X a 0x%04X\n",
                            opcode, cpu.pc - 2);
                    return -1;
            }
            break;

        default:
            fprintf(stderr, "Opcode inconnu : 0x%04X a 0x%04X\n",
                    opcode, cpu.pc - 2);
            return -1;
    }

    return 0;
}
