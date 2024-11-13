#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_rng.h>
#include <SDL2/SDL.h>

#define TITLE     "game of life"
#define WINDOW_X  0
#define WINDOW_Y  0
#define WINDOW_W  800
#define WINDOW_H  480

void
handle_error (const char *msg)
{
  perror (msg);
  exit (EXIT_FAILURE);
}

int
draw_grid (SDL_Renderer *renderer,
           unsigned int cell_width,
           unsigned int cell_height);

int
state_random_init (gsl_matrix *mat);

int
state_render (SDL_Renderer *renderer, gsl_matrix *current_state, SDL_Rect *rect);

int
state_update (gsl_matrix *next_state, gsl_matrix *current_state);

int
neighbor_count (gsl_matrix *current_state, size_t i, size_t j);

int
main (void)
{

  SDL_Renderer *renderer = NULL;
  SDL_Window *window = NULL;
  unsigned int cell_width, cell_height;
  cell_width  = 10;
  cell_height = 10;
  gsl_matrix *current_state = gsl_matrix_calloc (WINDOW_W / cell_width,
                                        WINDOW_H / cell_height);
  gsl_matrix *next_state = gsl_matrix_calloc (WINDOW_W / cell_width,
                                        WINDOW_H / cell_height);

  if (state_random_init (current_state) != 0)
    handle_error ("state_random_init");

  SDL_Rect cell = {0, 0, cell_width, cell_height};
  
  if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
      SDL_Quit ();
      handle_error ("SDL_Init");
    }

  window = SDL_CreateWindow (TITLE,
                             WINDOW_X, WINDOW_Y,
                             WINDOW_W, WINDOW_H,
                             0);
                             //SDL_WINDOW_BORDERLESS);
                             //SDL_WINDOW_OPENGL);
  if (window == NULL)
    {
      SDL_DestroyWindow (window);
      handle_error ("SDL_CreateWindow");
    }

  renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_SOFTWARE);
  if (renderer == NULL)
    {
      SDL_DestroyRenderer (renderer);
      handle_error ("SDL_CreateRenderer");
    }

  if (SDL_SetRenderDrawColor (renderer, 0xff, 0xff, 0xff, 0xff) != 0)
    handle_error ("SDL_SetRenderDrawColor");

  if (SDL_RenderClear (renderer) != 0)
    handle_error ("SDL_RenderClear");

  SDL_RenderPresent (renderer);

  SDL_Event event;
  unsigned int running = 1;
  while (running)
    {
      while (SDL_PollEvent (&event))
        {
          switch (event.type)
            {
            case SDL_QUIT:
              running = 0;
              break;
            case SDL_KEYDOWN:
              if (event.key.keysym.sym == SDLK_ESCAPE)
                running = 0;
              break;
            }
        }
      /*  loop begin  */
      SDL_RenderClear (renderer);
      if (state_render (renderer, current_state, &cell) != 0)
        handle_error ("state_render");

      SDL_RenderPresent (renderer);
      if (state_update (next_state, current_state) != 0)
        handle_error ("state_update");
      gsl_matrix_memcpy (current_state, next_state); 
      SDL_Delay (100);
      /*  loop end  */
    }
  SDL_RenderPresent (renderer);

  SDL_DestroyRenderer (renderer);
  SDL_DestroyWindow (window);
	SDL_Quit ();
	exit (EXIT_SUCCESS);
}

int
draw_grid (SDL_Renderer *renderer,
           unsigned int cell_width,
           unsigned int cell_height)
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

