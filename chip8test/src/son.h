/*
 * son.h
 * Gestion du son CHIP-8 via SDL2 Audio.
 *
 * Le CHIP-8 n'a qu'un seul son : un bip (buzzer).
 * Il joue tant que le timer sonore (compteurSon) est > 0.
 */

#ifndef SON_H_
#define SON_H_

/*
 * sonOuvrir()  — initialise SDL2 Audio et ouvre le périphérique.
 *               Retourne 0 si OK, -1 en cas d'erreur.
 *
 * sonFermer()  — ferme le périphérique audio et libère les ressources.
 *
 * sonJouer()   — active le bip (à appeler quand compteurSon > 0).
 *
 * sonCouper()  — coupe le bip (à appeler quand compteurSon == 0).
 */
int  sonOuvrir(void);
void sonFermer(void);
void sonJouer(void);
void sonCouper(void);

#endif /* SON_H_ */
