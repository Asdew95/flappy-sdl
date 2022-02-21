#ifndef FLAPPY_H
#define FLAPPY_H

#include <SDL2/SDL.h>
#include <stdint.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

extern SDL_Renderer *renderer;

extern uint64_t lose_time;
extern int score;

SDL_Texture *load_texture(const char *path);

void game_lose(void);

void die(int status);

#endif
