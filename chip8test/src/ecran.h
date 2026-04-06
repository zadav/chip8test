/*
 * ecran.h
 * Gestion de l'affichage SDL2 pour l'émulateur CHIP-8.
 *
 * Le CHIP-8 a un écran de 64 × 32 pixels monochrome.
 * On l'affiche dans une fenêtre 640 × 320 (facteur ×10).
 */

#ifndef ECRAN_H_
#define ECRAN_H_

#include <stdint.h>

/* Dimensions natives CHIP-8 */
#define ECRAN_LARGEUR  64
#define ECRAN_HAUTEUR  32

/* Facteur d'agrandissement pour la fenêtre */
#define ECRAN_ECHELLE  10

/*
 * Buffer de pixels : 1 octet par pixel.
 *   0 = éteint (noir)
 *   1 = allumé (blanc)
 *
 * Déclaré extern ici, défini dans ecran.c.
 * Le CPU y accède directement pour dessiner les sprites (opcode DRW).
 */
extern uint8_t pixels[ECRAN_HAUTEUR][ECRAN_LARGEUR];

/*
 * ecranOuvrir()  — crée la fenêtre SDL2 et le renderer.
 *                  Retourne 0 si OK, -1 en cas d'erreur.
 *
 * ecranFermer()  — libère les ressources SDL2.
 *
 * ecranEffacer() — met tous les pixels à 0 (opcode CLS).
 *
 * ecranAfficher()— pousse le buffer pixels[] vers la fenêtre.
 */
int  ecranOuvrir(void);
void ecranFermer(void);
void ecranEffacer(void);
void ecranAfficher(void);

#endif /* ECRAN_H_ */
