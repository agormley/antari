/***
 * Stella is the interface between the atari emulator and
 * the SDL framework.
 */

#include "includes.h"

int
StellaProcessEvent(SDL_Event event){

  switch(event.type){
  case SDL_QUIT:
    gQuit = true;
    break;
  case SDL_APP_TERMINATING:
    break;
  case SDL_APP_WILLENTERBACKGROUND:
    break;
  case SDL_APP_DIDENTERBACKGROUND:
    break;
  case SDL_APP_WILLENTERFOREGROUND:
    break;
  case SDL_APP_DIDENTERFOREGROUND:
    break;
  case SDL_WINDOWEVENT:
    break;
  case SDL_SYSWMEVENT:
    break;
  case SDL_KEYDOWN:
    // TODO: move to keypress event handle
    switch( event.key.keysym.sym ){
    case SDLK_F1:
      pia->game_select = true;
      break;
    case SDLK_F2:
      pia->game_reset = true;
      break;
    case SDLK_F3:
      pia->color = pia->color?false:true;
      break;
    case SDLK_F4:
      pia->p0difficulty = true;
      break;
    case SDLK_F5:
      pia->p1difficulty = true;
      break;
    case SDLK_LEFT:
      pia->p0left = true;
      break;
    case SDLK_RIGHT:
      pia->p0right = true;
      break;
    case SDLK_UP:
      pia->p0up = true;
      break;
    case SDLK_DOWN:
      pia->p0down = true;
      break;
    case SDLK_SPACE:
      pia->p0button = true;
      break;
    case SDLK_ESCAPE:
      gQuit = true;
      break;
    default:
      break;
    }
    break;
  case SDL_KEYUP:
    // TODO: move to keypress event handle
    switch( event.key.keysym.sym ){
    case SDLK_F1:
      pia->game_select = false;
      break;
    case SDLK_F2:
      pia->game_reset = false;
      break;
    }
    break;
  default:
    break;
  }
  
  return 0;
}

Uint32 StellaCreatePixel(unsigned char alpha,
			 unsigned char red,
			 unsigned char green,
			 unsigned char blue)
{
  Uint32 pixel = 0;
  pixel = pixel | ( blue << 0);
  pixel = pixel | ( green << 8);
  pixel = pixel | ( red << 16);
  pixel = pixel | ( alpha << 24);

  return pixel;
}

Uint32 pixels[STELLA_VERTICAL_CLOCK_COUNTS*STELLA_HORIZONTAL_LINES] = {0};

int
StellaPrintFrame(STELLA *stella, Uint32 framebuffer
		 [STELLA_HORIZONTAL_LINES]
		 [STELLA_VERTICAL_CLOCK_COUNTS])
{
  int ret = 0;
  SDL_Event event = {0};

  while( SDL_PollEvent(&event)){
    StellaProcessEvent(event);
  }

  ret = SDL_UpdateTexture(stella->texture, NULL, framebuffer, STELLA_VERTICAL_CLOCK_COUNTS  * sizeof (Uint32));

  if (ret)
    printf("[%s:%d]Error: %s\n",__FUNCTION__, __LINE__, SDL_GetError());

  ret = SDL_RenderClear(stella->renderer);

  if (ret)
    printf("[%s:%d]Error: %s\n",__FUNCTION__, __LINE__, SDL_GetError());

  ret = SDL_RenderCopy(stella->renderer, stella->texture, NULL, NULL);

  if (ret)
    printf("[%s:%d]Error: %s\n",__FUNCTION__, __LINE__, SDL_GetError());

  SDL_RenderPresent(stella->renderer);

  if (ret)
    printf("[%s:%d]Error: %s\n",__FUNCTION__, __LINE__, SDL_GetError());

  return ret;
}


STELLA*
StellaCreate()
{
  STELLA* tia = NULL;
  SDL_Window* window;
  SDL_Renderer *sdlRenderer;
  SDL_Texture *sdlTexture;

  tia = (STELLA*)calloc(1, sizeof(STELLA));

  if ( !tia )
    printf("%s: failed", __FUNCTION__);

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("Gormtari",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    800,
			    600,
			    SDL_WINDOW_OPENGL);

  sdlRenderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderPresent(sdlRenderer);

  SDL_Delay(10);

  sdlTexture = SDL_CreateTexture(sdlRenderer,
				 SDL_PIXELFORMAT_ARGB8888,
				 SDL_TEXTUREACCESS_STREAMING,
				 STELLA_VERTICAL_CLOCK_COUNTS,
				 STELLA_HORIZONTAL_LINES);
  
  tia->window = window;
  tia->texture = sdlTexture;
  tia->renderer = sdlRenderer;

  return tia;
}
