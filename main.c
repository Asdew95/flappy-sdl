#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "bird.h"
#include "flappy.h"
#include "pipe.h"
#include "ui.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

uint64_t lose_time = 0;
int score = 0;

void game_init(void);

void tick(double delta);
void draw(void);

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init SDL_INIT_VIDEO failed! SDL error: %s\n",
                SDL_GetError());
        die(1);
    }

    window = SDL_CreateWindow("Flappy SDL", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, 0);
    if (window == NULL) {
        fprintf(stderr, "Creating window failed! SDL error: %s\n",
                SDL_GetError());
        die(1);
    }

    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        fprintf(stderr, "IMG_Init failed with flags %08x! SDL error: %s\n",
                img_flags, SDL_GetError());
        die(1);
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init failed! SDL error: %s\n", TTF_GetError());
        die(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Could not create renderer! SDL error: %s\n",
                SDL_GetError());
        die(1);
    }

    SDL_SetRenderDrawColor(renderer, 0xfb, 0x71, 0xff, 0xff);

    game_init();

    uint64_t now = SDL_GetPerformanceCounter();
    uint64_t last;

    SDL_Event e;
    while (1) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                die(0);
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_q) {
                    die(0);
                }

                if (e.key.repeat == 0) {
                    ui_keydown(e.key.keysym.sym);
                }
            }
        }

        last = now;
        now = SDL_GetPerformanceCounter();
        double delta = (double) ((now - last)
                / (double) SDL_GetPerformanceFrequency());

        tick(delta);

        SDL_RenderClear(renderer);

        draw();

        SDL_RenderPresent(renderer);
    }

    die(0);
}

SDL_Texture *load_texture(const char *path)
{
    SDL_Surface *tmp = IMG_Load(path);
    if (tmp == NULL) {
        fprintf(stderr, "Failed to load image %s! SDL error: %s\n", path,
                IMG_GetError());
        die(1);
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if (tex == NULL) {
        fprintf(stderr, "Failed to create texture %s! SDL error: %s\n", path,
                SDL_GetError());
        die(1);
    }

    return tex;
}

void game_init(void)
{
    srand(time(NULL));
    bird_init();
    pipe_init();
    ui_init();
}

void tick(double delta)
{
    ui_tick(delta);
}

void draw(void)
{
    ui_draw();
}

void game_lose(void)
{
    bird_kill();
    ui_state = ui_gameover;
    lose_time = SDL_GetTicks64();
}

void die(int status)
{
    bird_destroy();
    pipe_destroy();
    ui_destroy();

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }

    if (window != NULL) {
        SDL_DestroyWindow(window);
    }

    IMG_Quit();
    SDL_Quit();

    exit(status);
}
