/*
 * son.c
 * Génération d'une onde carrée via SDL2 Audio.
 *
 * Principe :
 *   SDL2 Audio fonctionne par callback : SDL appelle notre fonction
 *   `remplirBuffer()` dès qu'il a besoin de nouveaux échantillons audio.
 *   On y génère une onde carrée simple à 440 Hz (note La).
 *
 * Onde carrée :
 *   ┌───┐   ┌───┐   ┌───┐
 *   │   │   │   │   │   │
 *   ┘   └───┘   └───┘   └──
 *   Une demi-période = FREQUENCE_SON / 2 échantillons à +AMPLITUDE,
 *   l'autre moitié à -AMPLITUDE.
 */

#include <stdio.h>
#include <SDL2/SDL.h>
#include "son.h"

/* Fréquence de la note (Hz) */
#define FREQUENCE_SON    440

/* Fréquence d'échantillonnage audio (échantillons/seconde) */
#define FREQ_ECHANT      44100

/* Volume de l'onde (0–32767 pour int16) */
#define AMPLITUDE        3000

/*
 * Nombre d'échantillons par demi-période.
 * Ex : 44100 / 440 / 2 ≈ 50 échantillons par demi-période.
 */
#define DEMI_PERIODE     (FREQ_ECHANT / FREQUENCE_SON / 2)

/* Identifiant du périphérique audio SDL2 */
static SDL_AudioDeviceID periph_audio = 0;

/* Phase courante de l'onde (position dans la demi-période) */
static int phase = 0;

/* État du son : 1 = actif, 0 = muet */
static int actif = 0;

/*
 * remplirBuffer()
 * ---------------
 * Callback appelé par SDL2 pour remplir le buffer audio.
 * `stream` : pointeur vers le buffer à remplir
 * `longueur` : nombre d'octets à produire
 *
 * On génère une onde carrée :
 *   - si phase < DEMI_PERIODE → +AMPLITUDE
 *   - sinon                  → -AMPLITUDE
 * Si le son est coupé, on remplit de zéros (silence).
 */
static void remplirBuffer(void *donnees, uint8_t *stream, int longueur)
{
    (void)donnees;  /* paramètre non utilisé */

    int16_t *buf = (int16_t *)stream;
    int nb_samples = longueur / (int)sizeof(int16_t);

    for (int i = 0; i < nb_samples; i++) {
        if (actif) {
            buf[i] = (phase < DEMI_PERIODE) ? AMPLITUDE : -AMPLITUDE;
            phase = (phase + 1) % (DEMI_PERIODE * 2);
        } else {
            buf[i] = 0;
            phase = 0;
        }
    }
}

/*
 * sonOuvrir()
 * -----------
 * Configure et ouvre le périphérique audio SDL2.
 *
 * SDL_AudioSpec décrit le format audio souhaité :
 *   freq     : fréquence d'échantillonnage (44100 Hz)
 *   format   : AUDIO_S16SYS = entiers signés 16 bits, ordre natif
 *   channels : 1 = mono
 *   samples  : taille du buffer (512 échantillons ≈ 11 ms de latence)
 *   callback : notre fonction remplirBuffer()
 */
int sonOuvrir(void)
{
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL Audio Init : %s\n", SDL_GetError());
        return -1;
    }

    SDL_AudioSpec spec;
    SDL_zero(spec);
    spec.freq     = FREQ_ECHANT;
    spec.format   = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples  = 512;
    spec.callback = remplirBuffer;

    periph_audio = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    if (periph_audio == 0) {
        fprintf(stderr, "SDL_OpenAudioDevice : %s\n", SDL_GetError());
        return -1;
    }

    /* Démarrer la lecture (le callback tourne mais produit du silence) */
    SDL_PauseAudioDevice(periph_audio, 0);
    return 0;
}

/*
 * sonFermer()
 * -----------
 * Ferme le périphérique audio et libère SDL Audio.
 */
void sonFermer(void)
{
    if (periph_audio != 0) {
        SDL_CloseAudioDevice(periph_audio);
        periph_audio = 0;
    }
}

/*
 * sonJouer() / sonCouper()
 * ------------------------
 * Active ou désactive la génération du son dans le callback.
 * Thread-safe : le flag `actif` est lu dans le thread audio SDL,
 * mais les écritures entières sur int sont atomiques sur x86.
 */
void sonJouer(void)  { actif = 1; }
void sonCouper(void) { actif = 0; }
