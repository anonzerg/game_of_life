#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_rng.h>
#include <SDL2/SDL.h>

#define TITLE "game of life"
#define WINDOW_X 0
#define WINDOW_Y 0
#define WINDOW_W 800
#define WINDOW_H 480

void
handle_error (const char *msg);

int
draw_grid (SDL_Renderer *renderer, unsigned int cell_width, unsigned int cell_height);

int
state_random_init (gsl_matrix *mat);

int
state_render (SDL_Renderer *renderer, gsl_matrix *current_state, SDL_Rect *rect);

int
state_update (gsl_matrix *next_state, gsl_matrix *current_state);

int
neighbor_count (gsl_matrix *current_state, size_t i, size_t j);

#endif  /* GAME_OF_LIFE_H  */

