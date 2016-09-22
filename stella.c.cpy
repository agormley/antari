#include "includes.h"

/* char ColorToCharMap [16][8] = */
/*   { {'#', '@', '%', '&', '!', '*', '!', ' '}, */
/*     {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}, */
/*     {'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'}, */
/*     {'q', 'r', 's', 't', 'u', 'v', 'w', 'x'}, */
/*     {'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F'}, */
/*     {'G', 'H', 'I', 'J', 'K', 'J', 'L', 'M'}, */
/*     {'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U'}, */
/*     {'V', 'W', 'X', 'Y', 'Z', '1', '2', '3'}, */
/*     {'#', '@', '%', '&', '!', '*', '!', ' '}, */
/*     {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}, */
/*     {'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'}, */
/*     {'q', 'r', 's', 't', 'u', 'v', 'w', 'x'}, */
/*     {'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F'}, */
/*     {'G', 'H', 'I', 'J', 'K', 'J', 'L', 'M'}, */
/*     {'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U'}, */
/*     {'V', 'W', 'X', 'Y', 'Z', '1', '2', '3'} */
/*   }; */

void textcolor(int attr, int fg, int bg);



//setup the putpixel function
void StellaPutPixel2(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 (*p)[surface->pitch];

    p = (Uint8(*)[surface->pitch])&((Uint8 *)surface->pixels)[0]; 

    //    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    p[x][y] = pixel;
   
    /* switch(bpp) { */
    /* case 1: */
    /*     p[x][y] = pixel; */
    /*     break; */

    /* case 2: */
    /*     *(Uint16 *)p = pixel; */
    /*     break; */

    /* case 3: */
    /*     if(SDL_BYTEORDER == SDL_BIG_ENDIAN) { */
    /*         p[0] = (pixel >> 16) & 0xff; */
    /*         p[1] = (pixel >> 8) & 0xff; */
    /*         p[2] = pixel & 0xff; */
    /*     } else { */
    /*         p[0] = pixel & 0xff; */
    /*         p[1] = (pixel >> 8) & 0xff; */
    /*         p[2] = (pixel >> 16) & 0xff; */
    /*     } */
    /*     break; */

    /* case 4: */
    /*     *(Uint32 *)p = pixel; */
    /*     break; */
    /* } */
}

Uint32 createPixel(char alpha, char red, char green, char blue)
{
  Uint32 pixel = 0;
  pixel = pixel | ( blue << 0);
  pixel = pixel | ( green << 8);
  pixel = pixel | ( red << 16);
  pixel = pixel | ( alpha << 24);
  
  return pixel;
}

extern int framebuffer[STELLA_VERTICAL_CLOCK_COUNTS][STELLA_HORIZONTAL_LINES];

void*
StellaLoopThd(void *threadid)
{
  long tid = (long)threadid;
  SDL_Window* window;
  
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
  SDL_Renderer *sdlRenderer = SDL_CreateRenderer(window, -1, 0);
  Uint32 white = createPixel(0xff, 0xff, 0xff, 0xff) ;
  Uint32 black = createPixel(0xff, 0xff, 0xff, 0xff) ;
  Uint32 red = createPixel(0xff, 0xff, 0x00, 0x00) ;
  int clkcount = 0;
  int line = 0;

  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderPresent(sdlRenderer);



  SDL_Delay(1000);

  SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer,
					      SDL_PIXELFORMAT_ARGB8888,
					      SDL_TEXTUREACCESS_STREAMING,
					      STELLA_VERTICAL_CLOCK_COUNTS,
					      STELLA_HORIZONTAL_LINES);

 

  bool isWhite = false;
  
    /* white = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff); */
  /* black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);   */
  bool programrunning = true;
  SDL_Event evt;
  Uint32 pixels[STELLA_VERTICAL_CLOCK_COUNTS*STELLA_HORIZONTAL_LINES] = {0};
  Uint32 (*p)[STELLA_HORIZONTAL_LINES];
  int itr = 0;

  //  p = (Uint32(*)[STELLA_HORIZONTAL_LINES])&((Uint32 *)pixels)[0]; 

  while(programrunning){
    itr = 0;
    SDL_Delay(100);
    SDL_Event event = {0}; 
    while( SDL_PollEvent(&event)){
      switch(event.type){
      case SDL_QUIT:
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

  
    }
    
    /* if(itr == 0) */
    /*   memset(pixels, '\0',  STELLA_VERTICAL_CLOCK_COUNTS*STELLA_HORIZONTAL_LINES); */
    /* pixels[itr] = red; //isWhite?white:black; */
    /* itr = (itr + 1)  % (STELLA_VERTICAL_CLOCK_COUNTS*STELLA_HORIZONTAL_LINES); */
    for (line = 0;line <  STELLA_HORIZONTAL_LINES; line++)
      {
	
	for (clkcount=0 ;clkcount < STELLA_VERTICAL_CLOCK_COUNTS; clkcount++)
	  {
	    if (framebuffer[clkcount][line])
	      pixels[itr] = black; //isWhite?white:black;
	    else
	      pixels[itr] = white; //isWhite?white:black;

	    itr = (itr + 1)  % (STELLA_VERTICAL_CLOCK_COUNTS*STELLA_HORIZONTAL_LINES);
    
	    
	    // StellaPutPixel(screen, clkcount, line, clkcount % 2 ?  black : white)  ;
	    
	    //   p[clkcount][line] = isWhite?red:black;
	    //pixels[line * STELLA_VERTICAL_CLOCK_COUNTS + clkcount] = isWhite?white:black;
	      
	    /* if( line % 20 < ) */
	    /*   printf("%s: %d,%d\n", __FUNCTION__, clkcount, line); */
	    /*   StellaPutPixel(screen, clkcount, line, white)  ; */
	    /* else */
	    /*   StellaPutPixel(screen, clkcount, line, black)  ; */
		  
	  }
      }
    /* isWhite = !isWhite; */
    int ret = 0;
    ret = SDL_UpdateTexture(sdlTexture, NULL, pixels, STELLA_VERTICAL_CLOCK_COUNTS  * sizeof (Uint32));
    if (ret)
      printf("[%s:%d]Error: %s\n",__FUNCTION__, __LINE__, SDL_GetError());
    ret = SDL_RenderClear(sdlRenderer);
    if (ret)
      printf("[%s:%d]Error: %s\n",__FUNCTION__, __LINE__, SDL_GetError());
    ret = SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
    if (ret)
      printf("[%s:%d]Error: %s\n",__FUNCTION__, __LINE__, SDL_GetError());
    SDL_RenderPresent(sdlRenderer);
    if (ret)
      printf("[%s:%d]Error: %s\n",__FUNCTION__, __LINE__, SDL_GetError());
    // this works just like SDL_Flip() in SDL 1.2
    /* if(itr % 1000 == 0) */
    /*   printf("%s:%s itr %d\n", __FILE__, __FUNCTION__, itr); */
    // show image for 2 seconds
    //SDL_Delay(50);
    /* SDL_WaitEvent(&evt); */
    /* if(evt.type == SDL_QUIT) */
    /*   programrunning = false; */
  }


  
  /* while(1) */
  /*   { */
  /*     int clkcount = 0; */
  /*     int line = 0; */
  /*     int color = 0; */
  /*     for (;clkcount < STELLA_VERTICAL_CLOCK_COUNTS; clkcount++) */
  /* 	{ */
  /* 	  textcolor(RESET, WHITE, (int)(color++ % 8)); */
  /* 	  for (;line < STELLA_HORIZONTAL_LINES; line++) */
  /* 	    { */
  /* 	      printf("."); */
  /* 	    } */
  /* 	  printf("\n"); */
  /* 	} */
      
  /*     printf("\e[1;1H\e[2J"); */
  /*     textcolor(RESET, WHITE, BLACK); */
  /*     usleep(16); */
  /*   } */
}


STELLA*
StellaCreate()
{
  STELLA* tia = NULL;
  pthread_t thread;
  int rc;
  long t;
  
  tia = (STELLA*)calloc(1, sizeof(STELLA));
  if ( !tia )
    printf("%s: failed", __FUNCTION__);
  rc = pthread_create(&thread, NULL, StellaLoopThd, (void *)t);
  if (rc){
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
  }
  

  /* bool running = true; */
  /* Uint32 start = 0; */
  /* const int FPS = 30; */

  /* while(running) { */
  /*   start = SDL_GetTicks(); */
  /*   SDL_Event event = {0}; */
  /*   while(SDL_PollEvent(&event)) { */
  /*     switch(event.type) { */
  /*     case SDL_QUIT: */
  /* 	running = false; */
  /* 	break; */
  /*     } */
  /*   } */

  /*   //logic */

  /*   //render */
  /*   //SDL_Flip(screen); */
  /*   if(1000/FPS > SDL_GetTicks()-start) { */
  /*     SDL_Delay(1000/FPS-(SDL_GetTicks()-start)); */
  /*   } */
  /* } */
  /* SDL_Quit(); */
  /* return 0; */
  
  return tia;
}


char *attrs[] = {"NORMAL", "BRIGHT", "DIM", "UNDERLINE", "BLINK",
		 "REVERSE", "HIDDEN", "EXIT"};
char *colors[] = {"BLACK", "RED", "GREEN", "YELLOW", "BLUE", "MAGENTA",
		  "CYAN", "WHITE", "EXIT"};



int print_menu(char *array[], int n_options, char *title);

int StellaPrintColors()
{

int attr, fg, bg;
int attr_size, colors_size;

attr_size = ARRAY_SIZE(attrs);
colors_size = ARRAY_SIZE(colors);
while(1)
  {printf("\n");
    attr = print_menu(attrs, attr_size, "Choose the attr you want:");
    if(attr == attr_size - 1)
      break;
    fg = print_menu(colors, colors_size, "Choose the foreground you want:");
    if(attr == colors_size - 1)
      break;
    bg = print_menu(colors, colors_size, "Choose the background you want:");
    if(attr == colors_size - 1)
      break;
    printf("\n");
    textcolor(attr, fg, bg);
    printf("This is what you get if you use the combination %s attribute %s foreground and %s"
	   " background", attrs[attr], colors[fg], colors[bg]);
    textcolor(RESET, WHITE, BLACK);
    system("clear");
  }
return 0;
}

int print_menu(char *array[], int n_options, char *title)
{int choice, i;
  for(i = 0;i < n_options; ++i)
    printf("%d.%s\n", i, array[i]);
  printf("%s", title);
  scanf("%d", &choice);
  return choice;
}
void textcolor(int attr, int fg, int bg)
{char command[13];

  /* Command is the control command to the terminal */
  sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
  printf("%s", command);
}

