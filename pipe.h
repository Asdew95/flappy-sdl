#ifndef PIPE_H
#define PIPE_H

#include "flappy.h"

#define PIPE_VELOCITY 120
#define PIPE_MAX_GAP 200
#define PIPE_MIN_GAP 120
#define PIPE_MIN_HEIGHT 60
#define PIPE_MAX_HEIGHT (SCREEN_HEIGHT - 300)
#define PIPE_INTERVAL 300
#define PIPE_WIDTH 64
#define PIPES_LOADED 3

// pipe_t is a set of two pipes: top and bottom
typedef struct {
    float x;
    int gap;
    int height;
} pipe_t;

extern pipe_t pipes[PIPES_LOADED];

void pipe_init(void);
void pipe_reset(void);
void pipe_destroy(void);

void pipe_tick(double delta);
void pipe_draw(void);

#endif
