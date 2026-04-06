/*
 * clavier.h
 * Gestion du clavier CHIP-8 (16 touches hexadécimales).
 *
 * Disposition originale CHIP-8 → mapping clavier PC moderne :
 *
 *   CHIP-8    Clavier PC
 *   1 2 3 C   1 2 3 4
 *   4 5 6 D   Q W E R
 *   7 8 9 E   A S D F
 *   A 0 B F   Z X C V
 */

#ifndef CLAVIER_H_
#define CLAVIER_H_

#include <stdint.h>
#include <SDL2/SDL.h>

/*
 * touches[16] : état de chaque touche CHIP-8 (0–F).
 *   1 = touche actuellement enfoncée
 *   0 = touche relâchée
 */
extern uint8_t touches[16];

/*
 * clavierTraiterEvenement(event)
 * ------------------------------
 * À appeler pour chaque SDL_Event reçu dans la boucle principale.
 * Met à jour le tableau touches[] en fonction des SDL_KEYDOWN / SDL_KEYUP.
 * Retourne 1 si l'événement était une touche CHIP-8, 0 sinon.
 */
int clavierTraiterEvenement(const SDL_Event *event);

/*
 * clavierToucheVersChip8(scancode)
 * ---------------------------------
 * Convertit un scancode SDL en index de touche CHIP-8 (0–15).
 * Retourne -1 si la touche ne correspond à aucune touche CHIP-8.
 */
int clavierToucheVersChip8(SDL_Scancode scancode);

#endif /* CLAVIER_H_ */
