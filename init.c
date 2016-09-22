#include "init.h"

SDL_Window* init(char *title, int x, int y)
{
  SDL_Window *window = NULL;
  /* Initialise SDL Video */
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      printf("Could not initialize SDL: %s\n", SDL_GetError());
		
      goto end;
    }

  window = SDL_CreateWindow(title,
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    x, y,
			    0);
  if (window == NULL)
    {
      printf("Couldn't set window mode to %d x %d: %s\n",
	     x, y, SDL_GetError());
      goto end;
    }
    
 end:
  return window;
	
}

void cleanup()
{
	/* Shut down SDL */
	
	SDL_Quit();
}
