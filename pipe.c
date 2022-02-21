#include <SDL2/SDL.h>
#include <stdlib.h>

#include "flappy.h"
#include "pipe.h"

SDL_Texture *pipe_texture = NULL;
SDL_Texture *pipe_head_texture = NULL;

pipe_t pipes[PIPES_LOADED];

static pipe_t pipe_gen(float x);

void pipe_init(void)
{
    pipe_texture = load_texture("assets/pipe.png");
    pipe_head_texture = load_texture("assets/pipe-head.png");

    pipe_reset();
}

static pipe_t pipe_gen(float x)
{
    pipe_t pipe;
    pipe.x = x;
    pipe.gap = rand() % (PIPE_MAX_GAP - PIPE_MIN_GAP) + PIPE_MIN_GAP;
    pipe.height = rand() % (PIPE_MAX_HEIGHT - PIPE_MIN_HEIGHT)
        + PIPE_MIN_HEIGHT;

    return pipe;
}

void pipe_tick(double delta)
{
    for (int i = 0; i < PIPES_LOADED; i++) {
        pipes[i].x -= PIPE_VELOCITY * delta;

        if (pipes[i].x <= -PIPE_WIDTH) {
            pipes[i] = pipe_gen(PIPE_INTERVAL * PIPES_LOADED);
        }
    }
}

void pipe_draw(void)
{
    SDL_FRect pos;
    pos.w = PIPE_WIDTH;
    pos.h = 64;

    for (int i = 0; i < PIPES_LOADED; i++) {
        pos.x = pipes[i].x;
        pos.y = pipes[i].height + pipes[i].gap;

        SDL_RenderCopyF(renderer, pipe_head_texture, NULL, &pos);
        while (pos.y + 64 <= SCREEN_HEIGHT) {
            pos.y += 64;
            SDL_RenderCopyF(renderer, pipe_texture, NULL, &pos);
        }

        pos.y = pipes[i].height - 64;
        SDL_RenderCopyExF(renderer, pipe_head_texture, NULL, &pos, 180, NULL,
                SDL_FLIP_NONE);
        while (pos.y >= 0) {
            pos.y -= 64;
            SDL_RenderCopyF(renderer, pipe_texture, NULL, &pos);
        }
    }
}

void pipe_reset(void)
{
    for (int i = 0; i < PIPES_LOADED; i++) {
        pipes[i] = pipe_gen(PIPE_INTERVAL * (i + 1));
    }
}

void pipe_destroy(void)
{
    if (pipe_texture != NULL) {
        SDL_DestroyTexture(pipe_texture);
    }

    if (pipe_head_texture != NULL) {
        SDL_DestroyTexture(pipe_head_texture);
    }
}
