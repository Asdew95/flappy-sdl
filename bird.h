#ifndef BIRD_H
#define BIRD_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define BIRD_X_OFFSET 120
#define BIRD_DIAMETER 48
#define BIRD_GRAVITY 700
#define BIRD_JUMP_VEL 350

typedef struct {
    SDL_FRect pos;
    struct {
        float x;
        float y;
    } vel;
    bool dead;
} bird_t;

void bird_init(void);
void bird_kill(void);
void bird_reset(void);
void bird_destroy(void);

void bird_draw();
void bird_tick(double delta);

void bird_keydown(SDL_Keycode key);

#endif
