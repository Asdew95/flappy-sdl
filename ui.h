#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>

typedef struct {
    int width;
    int height;
    SDL_Texture *texture;
} text_t;

extern enum ui_state {
    ui_start,
    ui_game,
    ui_gameover
} ui_state;

void ui_init(void);
void ui_destroy(void);

void ui_keydown(SDL_Keycode key);

void ui_tick(double delta);
void ui_draw(void);
void update_score(void);

#endif
