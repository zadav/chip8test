/*
 * ecran.c
 * Affichage SDL2 — fenêtre 640×320, buffer 64×32.
 */

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "ecran.h"

/* ── Buffer de pixels (accessible depuis cpu_exec.c via extern) ── */
uint8_t pixels[ECRAN_HAUTEUR][ECRAN_LARGEUR];

/* ── Variables SDL2 privées à ce fichier ── */
static SDL_Window   *fenetre  = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture  *texture  = NULL;

/*
 * ecranOuvrir()
 * -------------
 * Initialise SDL2, crée la fenêtre, le renderer et la texture.
 *
 * La texture est en format ARGB8888 :
 *   chaque pixel = 4 octets (Alpha, Rouge, Vert, Bleu).
 * On utilise SDL_TEXTUREACCESS_STREAMING pour pouvoir la mettre
 * à jour manuellement à chaque frame.
 */
int ecranOuvrir(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init : %s\n", SDL_GetError());
        return -1;
    }

    fenetre = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        ECRAN_LARGEUR  * ECRAN_ECHELLE,
        ECRAN_HAUTEUR  * ECRAN_ECHELLE,
        0
    );
    if (!fenetre) {
        fprintf(stderr, "SDL_CreateWindow : %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer : %s\n", SDL_GetError());
        return -1;
    }

    /*
     * La texture fait la taille NATIVE (64×32).
     * SDL se charge de l'étirer à 640×320 lors du rendu.
     */
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        ECRAN_LARGEUR,
        ECRAN_HAUTEUR
    );
    if (!texture) {
        fprintf(stderr, "SDL_CreateTexture : %s\n", SDL_GetError());
        return -1;
    }

    ecranEffacer();
    return 0;
}

/*
 * ecranFermer()
 * -------------
 * Libère toutes les ressources SDL2 dans l'ordre inverse de création.
 */
void ecranFermer(void)
{
    if (texture)  SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (fenetre)  SDL_DestroyWindow(fenetre);
    SDL_Quit();
}

/*
 * ecranEffacer()
 * --------------
 * Remet tous les pixels à 0. Appelé par l'opcode CLS (0x00E0).
 */
void ecranEffacer(void)
{
    memset(pixels, 0, sizeof(pixels));
}

/*
 * ecranAfficher()
 * ---------------
 * Convertit le buffer pixels[] (1 bit par pixel) en une texture ARGB
 * puis l'affiche dans la fenêtre.
 *
 * Pixels allumés  → blanc (0xFFFFFFFF)
 * Pixels éteints  → noir  (0xFF000000)
 */
void ecranAfficher(void)
{
    /*
     * SDL_UpdateTexture attend un tableau de pixels 32 bits.
     * On convertit notre buffer 1 octet → 4 octets ARGB.
     */
    uint32_t buf[ECRAN_HAUTEUR][ECRAN_LARGEUR];

    for (int y = 0; y < ECRAN_HAUTEUR; y++)
        for (int x = 0; x < ECRAN_LARGEUR; x++)
            buf[y][x] = pixels[y][x] ? 0xFFFFFFFF : 0xFF000000;

    SDL_UpdateTexture(texture, NULL, buf, ECRAN_LARGEUR * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
