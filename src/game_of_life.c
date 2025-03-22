#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game_of_life.h"

void
handle_error (const char *msg)
{
  perror (msg);
  exit (EXIT_FAILURE);
}

int
draw_grid (SDL_Renderer *renderer, unsigned int cell_width, unsigned int cell_height)
{
  if (SDL_SetRenderDrawColor (renderer, 0x80, 0x80, 0x80, 0xff) != 0)
    return -1;

  SDL_Rect rect;
  rect.w = cell_width;
  rect.h = cell_height;

  unsigned int i, j;
  for (i = 0; i < WINDOW_W / cell_width; i++)
    {
      rect.x = i * cell_width;
      for (j = 0; j < WINDOW_H / cell_height; j++)
        {
          rect.y = j * cell_height;
          if (SDL_RenderDrawRect (renderer, &rect) != 0)
            return -1;
        }
    }
  return 0;
}

int
state_render (SDL_Renderer *renderer, gsl_matrix *current_state, SDL_Rect *rect)
{
  unsigned int i, j;
  for (i = 0; i < current_state -> size1; i++)
    for (j = 0; j < current_state -> size2; j++)
      {
        rect -> x = i * rect -> w;
        rect -> y = j * rect -> h;
        if (gsl_matrix_get (current_state, i, j) == 0)
          {
            if (SDL_SetRenderDrawColor (renderer, 0xff, 0xff, 0xff, 0xff) != 0)
              return -1;
            if (SDL_RenderDrawRect (renderer, rect) != 0)
              return -1;
          }
        else if (gsl_matrix_get (current_state, i, j) == 1)
          {
            if (SDL_SetRenderDrawColor (renderer, 0x00, 0x00, 0x00, 0xff) != 0)
              return -1;
            if (SDL_RenderFillRect (renderer, rect) != 0)
              return -1;
          }
        else
          {
            return -2;
          }
      }
  return 0;
}

int
state_random_init (gsl_matrix *mat)
{
  const gsl_rng_type *T;
  gsl_rng *r;
  gsl_rng_env_setup ();
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);
  if (r == NULL)
    return -1;

  size_t i, j;
  for (i = 0; i < mat -> size1; i++)
    for (j = 0; j < mat -> size2; j++)
      gsl_matrix_set (mat, i, j, gsl_rng_uniform_int (r, 2));

  gsl_rng_free (r);
  return 0;
}

int
state_update (gsl_matrix *next_state, gsl_matrix *current_state)
{
  int live_neighbors;
  int current_cell;
  size_t i, j;
  for (i = 0; i < current_state -> size1; i++)
    for (j = 0; j < current_state -> size2; j++)
      {
        live_neighbors = neighbor_count (current_state, i, j);
        current_cell = (int)gsl_matrix_get (current_state, i, j);
        if (current_cell == 1)
          {
            if (live_neighbors == 2 || live_neighbors == 3)
              gsl_matrix_set (next_state, i, j, 1);
            else if (live_neighbors < 2 || live_neighbors > 3)
              gsl_matrix_set (next_state, i, j, 0);
          }
        else if (current_cell == 0)
          {
            if (live_neighbors == 3)
              gsl_matrix_set (next_state, i, j, 1);
          }
        else
          return -1;
      }
  return 0;
}

int
neighbor_count (gsl_matrix *mat, size_t i, size_t j)
{
  int count = 0;
  int dir[8][2] = {
        {-1, -1}, {-1, 0}, {-1, +1},
        { 0, -1},          { 0, +1},
        {+1, -1}, {+1, 0}, {+1, +1},
  };
  size_t k, ni, nj;
  for (k = 0; k < 8; k++)
    {
      ni = i + dir[k][0];
      nj = j + dir[k][1];
      if (ni >= 0 && ni < mat -> size1 && nj >= 0 && nj < mat -> size2)
        {
          if ((int)gsl_matrix_get (mat, ni, nj) == 1)
            count++;
        }
    }
  return count;
}

