/*
 * cpu.h
 *
 *  Created on: 11 d�c. 2012
 *      Author: xdz
 */

#ifndef CPU_H_
#define CPU_H_

#define TAILLEMEMOIRE 4096
#define ADRESSEDEBUT 512


    typedef struct
    {
        Uint8 memoire[TAILLEMEMOIRE];
        Uint8 V[16]; //le registre
        Uint16 I; //stocke une adresse m�moire ou dessinateur
        Uint16 saut[16]; //pour g�rer les sauts dans � m�moire �, 16 au maximum
        Uint8 nbrsaut; //stocke le nombre de sauts effectu�s pour ne pas d�passer 16
        Uint8 compteurJeu; //compteur pour la synchronisation
        Uint8 compteurSon; //compteur pour le son
        Uint16 pc; //pour parcourir le tableau � m�moire �
    } CPU;

CPU cpu;  //d�claration de notre CPU

void initialiserCpu() ;
void decompter() ;

#endif /* CPU_H_ */
