#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "bird.h"
#include "flappy.h"
#include "pipe.h"
#include "ui.h"

enum ui_state ui_state;

TTF_Font *ui_font = NULL;
TTF_Font *score_font = NULL;

text_t start_text;
text_t gameover_text;
text_t score_text;

static text_t create_text(const char *text, SDL_Color color);
static text_t create_textf(const char *text, SDL_Color color, TTF_Font *font);
static TTF_Font *load_font(const char *path, int size);

void ui_init(void)
{
    ui_state = ui_start;
    ui_font = load_font("assets/boxfont.ttf", 48);
    score_font = load_font("assets/boxfont.ttf", 42);
    update_score();
    start_text = create_text("PRESS UP TO PLAY",
            (SDL_Color) { 255, 255, 255, 255 });
    gameover_text = create_text("GAME OVER",
            (SDL_Color) { 255, 255, 255, 255 });
}

void ui_keydown(SDL_Keycode key)
{
    if (ui_state == ui_start) {
        if (key == SDLK_UP) {
            ui_state = ui_game;
        }
    } else if (ui_state == ui_gameover) {
        if (SDL_GetTicks64() > lose_time + 1000) {
            ui_state = ui_start;
            bird_reset();
            pipe_reset();
            score = 0;
            update_score();
        }
    }

    bird_keydown(key);
}

void ui_tick(double delta)
{
    if (ui_state == ui_game || ui_state == ui_gameover) {
        bird_tick(delta);
        pipe_tick(delta);
    }
}

void ui_draw(void)
{
    pipe_draw();
    bird_draw();

    if (ui_state == ui_start) {
        SDL_Rect pos;
        pos.x = (SCREEN_WIDTH - start_text.width) / 2;
        pos.y = SCREEN_HEIGHT - 100;
        pos.w = start_text.width;
        pos.h = start_text.height;
        SDL_RenderCopy(renderer, start_text.texture, NULL, &pos);
    } else if (ui_state == ui_gameover) {
        if (SDL_GetTicks64() > lose_time + 200) {
            SDL_Rect pos;
            pos.x = (SCREEN_WIDTH - gameover_text.width) / 2;
            pos.y = 100;
            pos.w = gameover_text.width;
            pos.h = gameover_text.height;
            SDL_RenderCopy(renderer, gameover_text.texture, NULL, &pos);
        }
    }

    SDL_Rect spos;
    spos.x = (SCREEN_WIDTH - score_text.width) / 2;
    spos.y = 10;
    spos.w = score_text.width;
    spos.h = score_text.height;
    SDL_RenderCopy(renderer, score_text.texture, NULL, &spos);
}

void update_score(void)
{
    if (score_text.texture != NULL) {
        SDL_DestroyTexture(score_text.texture);
    }

    char text[12];
    sprintf(text, "%d", score);
    score_text = create_textf(text, (SDL_Color) { 255, 255, 255, 255 },
            score_font);
}

static text_t create_text(const char *text, SDL_Color color)
{
    return create_textf(text, color, ui_font);
}

static text_t create_textf(const char *textstr, SDL_Color color, TTF_Font *font)
{
    text_t text;

    SDL_Surface *tmp = TTF_RenderText_Solid(font, textstr, color);
    if (tmp == NULL) {
        fprintf(stderr, "Could not create text! SDL error: %s\n",
                TTF_GetError());
        die(1);
    }

    text.texture = SDL_CreateTextureFromSurface(renderer, tmp);
    text.width = tmp->w;
    text.height = tmp->h;
    SDL_FreeSurface(tmp);

    if (text.texture == NULL) {
        fprintf(stderr, "Could not create texture from text! SDL error: %s\n",
                SDL_GetError());
        die(1);
    }

    return text;
}

static TTF_Font *load_font(const char *path, int size)
{
    TTF_Font *font = TTF_OpenFont(path, size);
    if (font == NULL) {
        fprintf(stderr, "Could not load font %s! SDL error: %s\n", path,
                TTF_GetError());
        die(1);
    }

    return font;
}

void ui_destroy(void)
{
    if (start_text.texture != NULL) {
        SDL_DestroyTexture(start_text.texture);
    }

    if (gameover_text.texture != NULL) {
        SDL_DestroyTexture(gameover_text.texture);
    }

    if (score_text.texture != NULL) {
        SDL_DestroyTexture(score_text.texture);
    }

    TTF_CloseFont(ui_font);

    TTF_Quit();
}
