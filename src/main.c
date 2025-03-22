#include "game_of_life.h"

int
main (void)
{
  SDL_Renderer *renderer = NULL;
  SDL_Window *window = NULL;
  unsigned int cell_width, cell_height;
  cell_width  = 10;
  cell_height = 10;
  gsl_matrix *current_state = gsl_matrix_calloc (WINDOW_W / cell_width, WINDOW_H / cell_height);
  gsl_matrix *next_state = gsl_matrix_calloc (WINDOW_W / cell_width, WINDOW_H / cell_height);

  if (state_random_init (current_state) != 0)
    handle_error ("state_random_init");

  SDL_Rect cell = {0, 0, cell_width, cell_height};
  
  if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
      SDL_Quit ();
      handle_error ("SDL_Init");
    }

  window = SDL_CreateWindow (TITLE, WINDOW_X, WINDOW_Y, WINDOW_W, WINDOW_H, 0);
  
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

