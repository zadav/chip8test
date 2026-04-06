/*
 * clavier.c
 * Mapping clavier PC → touches CHIP-8 et mise à jour de l'état.
 */

#include "clavier.h"

/* État des 16 touches CHIP-8 (accessible via extern depuis cpu_exec.c) */
uint8_t touches[16];

/*
 * Table de correspondance : scancode SDL → index touche CHIP-8 (0–F).
 *
 * Disposition :
 *   Clavier PC :  1  2  3  4  Q  W  E  R  A  S  D  F  Z  X  C  V
 *   Touche C8  :  1  2  3  C  4  5  6  D  7  8  9  E  A  0  B  F
 */
static const struct {
    SDL_Scancode scancode;
    uint8_t      touche_chip8;
} mapping[] = {
    { SDL_SCANCODE_1, 0x1 }, { SDL_SCANCODE_2, 0x2 },
    { SDL_SCANCODE_3, 0x3 }, { SDL_SCANCODE_4, 0xC },
    { SDL_SCANCODE_Q, 0x4 }, { SDL_SCANCODE_W, 0x5 },
    { SDL_SCANCODE_E, 0x6 }, { SDL_SCANCODE_R, 0xD },
    { SDL_SCANCODE_A, 0x7 }, { SDL_SCANCODE_S, 0x8 },
    { SDL_SCANCODE_D, 0x9 }, { SDL_SCANCODE_F, 0xE },
    { SDL_SCANCODE_Z, 0xA }, { SDL_SCANCODE_X, 0x0 },
    { SDL_SCANCODE_C, 0xB }, { SDL_SCANCODE_V, 0xF },
};

#define NB_TOUCHES (int)(sizeof(mapping) / sizeof(mapping[0]))

/*
 * clavierToucheVersChip8()
 * ------------------------
 * Parcourt la table de mapping pour trouver l'index CHIP-8
 * correspondant au scancode SDL reçu.
 */
int clavierToucheVersChip8(SDL_Scancode scancode)
{
    for (int i = 0; i < NB_TOUCHES; i++)
        if (mapping[i].scancode == scancode)
            return mapping[i].touche_chip8;
    return -1;
}

/*
 * clavierTraiterEvenement()
 * -------------------------
 * Met à jour touches[] selon les événements SDL_KEYDOWN et SDL_KEYUP.
 */
int clavierTraiterEvenement(const SDL_Event *event)
{
    if (event->type != SDL_KEYDOWN && event->type != SDL_KEYUP)
        return 0;

    int idx = clavierToucheVersChip8(event->key.keysym.scancode);
    if (idx < 0)
        return 0;

    touches[idx] = (event->type == SDL_KEYDOWN) ? 1 : 0;
    return 1;
}
