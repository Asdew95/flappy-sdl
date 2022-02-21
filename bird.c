#include <SDL2/SDL.h>
#include <stdbool.h>

#include "bird.h"
#include "flappy.h"
#include "pipe.h"
#include "ui.h"

SDL_Texture *bird_texture = NULL;
bird_t bird;
int next_pipe = 0;

void bird_init(void)
{
    bird_reset();

    if (bird_texture == NULL) {
        bird_texture = load_texture("assets/bird.png");
    }
}

void bird_keydown(SDL_Keycode key)
{
    if (key == SDLK_UP && !bird.dead) {
        bird.vel.y = -BIRD_JUMP_VEL;
    }
}

void bird_tick(double delta)
{
    bird.vel.y += BIRD_GRAVITY * delta;
    bird.pos.y += bird.vel.y * delta;

    if (!bird.dead) {
        if (bird.pos.y <= 0) {
            bird.vel.y = 0;
            bird.pos.y = 0;
        }

        if (bird.pos.y >= SCREEN_HEIGHT - BIRD_DIAMETER) {
            game_lose();
        }

        // Detect passing pipes
        if (pipes[next_pipe].x < bird.pos.x + 12) {
            score++;
            update_score();
            next_pipe = (next_pipe + 1) % PIPES_LOADED;
        }

        // Collision detection
        for (int i = 0; i < PIPES_LOADED; i++) {
            pipe_t pipe = pipes[i];

            // +4 and -4 to make hitbox slightly smaller
            if ((bird.pos.x > pipe.x + 4 && bird.pos.x < pipe.x
                        + PIPE_WIDTH - 4)
                    || (bird.pos.x + BIRD_DIAMETER - 4 > pipe.x
                        && bird.pos.x + BIRD_DIAMETER < pipe.x
                        + PIPE_WIDTH - 4)) {
                if (bird.pos.y < pipe.height - 4) {
                    game_lose();
                } else if (bird.pos.y + BIRD_DIAMETER > pipe.height
                        + pipe.gap + 4) {
                    game_lose();
                }
            }
        }
    }
}

void bird_draw(void)
{
    SDL_RenderCopyF(renderer, bird_texture, NULL, &bird.pos);
}

void bird_kill(void)
{
    bird.dead = true;
    bird.vel.y = -400;
}

void bird_reset(void)
{
    bird.pos = (SDL_FRect) { .x = BIRD_X_OFFSET, .y = 240, .w = BIRD_DIAMETER,
        .h = BIRD_DIAMETER };
    bird.vel.x = 0;
    bird.vel.y = 0;
    bird.dead = false;
    next_pipe = 0;
}

void bird_destroy(void)
{
    if (bird_texture != NULL) {
        SDL_DestroyTexture(bird_texture);
    }
}
