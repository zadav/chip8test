/*
 * cpu_exec.h
 * Déclaration du cycle fetch/decode/execute du CPU CHIP-8.
 */

#ifndef CPU_EXEC_H_
#define CPU_EXEC_H_

/*
 * executerInstruction()
 * ---------------------
 * Effectue un cycle complet :
 *   1. Fetch   : lit 2 octets à cpu.memoire[cpu.pc]
 *   2. Decode  : identifie l'opcode via le nibble de poids fort
 *   3. Execute : effectue l'opération et met à jour le PC
 *
 * Retourne 0 si l'instruction a été exécutée normalement.
 * Retourne -1 si l'opcode est inconnu ou si une erreur survient.
 */
int executerInstruction(void);

#endif /* CPU_EXEC_H_ */
