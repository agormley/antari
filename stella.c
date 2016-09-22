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
      // set reset
      break;
    default:
      break;
    }
    break;
  case SDL_KEYUP:
    break;
  case SDL_TEXTEDITING:
    break;
  case SDL_TEXTINPUT:
    break;
  case SDL_KEYMAPCHANGED:
    break;
  case SDL_MOUSEMOTION:
    break;
  case SDL_MOUSEBUTTONDOWN:
    break;
  case SDL_MOUSEBUTTONUP:
    break;
  case SDL_MOUSEWHEEL:
    break;
  case SDL_JOYAXISMOTION:
    break;
  case SDL_JOYBALLMOTION:
    break;
  case SDL_JOYHATMOTION:
    break;
  case SDL_JOYBUTTONDOWN:
    break;
  case SDL_JOYBUTTONUP:
    break;
  case SDL_JOYDEVICEADDED:
    break;
  case SDL_JOYDEVICEREMOVED:
    break;
  case SDL_CONTROLLERAXISMOTION:
    break;
  case SDL_CONTROLLERBUTTONDOWN:
    break;
  case SDL_CONTROLLERBUTTONUP:
    break;
  case SDL_CONTROLLERDEVICEADDED:
    break;
  case SDL_CONTROLLERDEVICEREMOVED:
    break;
  case SDL_CONTROLLERDEVICEREMAPPED:
    break;
  case SDL_FINGERDOWN:
    break;
  case SDL_FINGERUP:
    break;
  case SDL_FINGERMOTION:
    break;
  case SDL_DOLLARGESTURE:
    break;
  case SDL_MULTIGESTURE:
    break;
  case SDL_CLIPBOARDUPDATE:
    break;
  case SDL_DROPFILE:
    break;
  case SDL_AUDIODEVICEADDED:
    break;
  case SDL_AUDIODEVICEREMOVED:
    break;
  case SDL_RENDER_TARGETS_RESET:
    break;
  case SDL_RENDER_DEVICE_RESET:
    break;
  case SDL_USEREVENT:
    break;
  case SDL_LASTEVENT:
    break;
  default:
    break;
  }
  if (event.type==SDL_QUIT)
    return 0;
  else
    printf( "event type:%d\n", event.type );    /*     switch(event.type) { */

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
		 [STELLA_VERTICAL_CLOCK_COUNTS]
		 [STELLA_HORIZONTAL_LINES])
{
  //  SDL_Event evt;
  /* Uint32 (*p)[STELLA_HORIZONTAL_LINES]; */
  int itr = 0;
  /* Uint32 white = StellaCreatePixel(0xff, 0xff, 0xff, 0xff) ; */
  /* Uint32 black = StellaCreatePixel(0x00, 0x00, 0x00, 0x00) ; */
  //  Uint32 red = StellaCreatePixel(0xff, 0xff, 0x00, 0x00) ;
  int clkcount = 0;
  int line = 0;
  //  sdlRenderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(stella->renderer, 255, 0, 255, 255);
  SDL_RenderClear(stella->renderer);
  //  SDL_RenderPresent(sdlRenderer);
  
  /* // not working? must create renderer here? */
  /* SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer, */
  /* 					      SDL_PIXELFORMAT_ARGB8888, */
  /* 					      SDL_TEXTUREACCESS_STREAMING, */
  /* 					      STELLA_VERTICAL_CLOCK_COUNTS, */
  /* 					      STELLA_HORIZONTAL_LINES); */

  
  int ret = 0;
  itr = 0;
  SDL_Delay(100);
  SDL_Event event = {0}; 
  while( SDL_PollEvent(&event)){
    StellaProcessEvent(event);
  }
  
  /* if(itr == 0) */
  /*   memset(pixels, '\0',  STELLA_VERTICAL_CLOCK_COUNTS*STELLA_HORIZONTAL_LINES); */
  /* pixels[itr] = red; //isWhite?white:black; */
  /* itr = (itr + 1)  % (STELLA_VERTICAL_CLOCK_COUNTS*STELLA_HORIZONTAL_LINES); */
  for (line = 0; line < STELLA_HORIZONTAL_LINES; line++)
    {
	
      for (clkcount=0 ;clkcount < STELLA_VERTICAL_CLOCK_COUNTS; clkcount++)
	{
	  pixels[itr] = framebuffer[clkcount][line];
	  /* if (framebuffer[clkcount][line]) */
	  /*   pixels[itr] = black; //isWhite?white:black; */
	  /* else */
	  /*   pixels[itr] = white; //isWhite?white:black; */

	  itr = (itr + 1)  % (STELLA_VERTICAL_CLOCK_COUNTS*STELLA_HORIZONTAL_LINES);
    
	}
    }
  ret = SDL_UpdateTexture(stella->texture, NULL, pixels, STELLA_VERTICAL_CLOCK_COUNTS  * sizeof (Uint32));
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
  // this works just like SDL_Flip() in SDL 1.2

  //  SDL_DestroyTexture(sdlTexture);
  return itr;
}


STELLA*
StellaCreate()
{
  STELLA* tia = NULL;
  //  pthread_t thread;
  //  int rc;
  //  long t;

  SDL_Window* window;
  SDL_Renderer *sdlRenderer;
  SDL_Texture *sdlTexture;

  
  tia = (STELLA*)calloc(1, sizeof(STELLA));
  if ( !tia )
    printf("%s: failed", __FUNCTION__);


  
  SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

  // Create an application window with the following settings:
  window = SDL_CreateWindow("Gormtari",                  // window title
			    SDL_WINDOWPOS_UNDEFINED,           // initial x position
			    SDL_WINDOWPOS_UNDEFINED,           // initial y position
			    800,
			    600,
			    /* STELLA_VERTICAL_CLOCK_COUNTS*4,      // width, in pixels */
			    /* STELLA_HORIZONTAL_LINES*4,           // height, in pixels */
			    SDL_WINDOW_OPENGL                  // flags - see below
			    );
  /* SDL_Window* window = init("Stella Window", */
  /* 			    STELLA_HORIZONTAL_LINES*2, */
  /* 			    STELLA_VERTICAL_CLOCK_COUNTS*2); */
  /* SDL_Surface *screen = NULL; */
  /* screen = SDL_GetWindowSurface(window); */
  sdlRenderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderPresent(sdlRenderer);



  SDL_Delay(1000);

  sdlTexture = SDL_CreateTexture(sdlRenderer,
					      SDL_PIXELFORMAT_ARGB8888,
					      SDL_TEXTUREACCESS_STREAMING,
					      STELLA_VERTICAL_CLOCK_COUNTS,
					      STELLA_HORIZONTAL_LINES);

  tia->window = window;
  tia->texture = sdlTexture;
  tia->renderer = sdlRenderer;
  
  /* rc = pthread_create(&thread, NULL, StellaLoopThd, (void *)t); */
  /* if (rc){ */
  /*   printf("ERROR; return code from pthread_create() is %d\n", rc); */
  /*   exit(-1); */
  /* } */
  
  return tia;
}

void textcolor(int attr, int fg, int bg)
{char command[13];

  /* Command is the control command to the terminal */
  sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
  printf("%s", command);
}

