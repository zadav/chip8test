/*
 * rom.h
 * Déclaration de la fonction de chargement de ROM CHIP-8.
 */

#ifndef ROM_H_
#define ROM_H_

/*
 * chargerRom(chemin)
 * ------------------
 * Ouvre le fichier .ch8 situé à `chemin` et le copie dans
 * cpu.memoire à partir de ADRESSE_DEBUT (0x200).
 *
 * Retourne  0 si le chargement a réussi.
 * Retourne -1 en cas d'erreur (fichier absent, trop grand, etc.).
 *
 * Taille maximale d'une ROM : TAILLE_MEMOIRE - ADRESSE_DEBUT = 3584 octets.
 */
int chargerRom(const char *chemin);

#endif /* ROM_H_ */
