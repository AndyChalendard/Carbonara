#include <stdlib.h>
#include <SDL2/SDL.h>

#ifndef __MAP_HEADER__
#define __MAP_HEADER__

typedef struct {
   int         w; /* longueur */
   int         h; /* hauteur  */
   block_t **  map;
   charac_t *  ennemies;
   int         nbEnnemies;
} map_t;

typedef struct {
   int           id;
   SDL_Texture * t;
   int           opt; /* option : franchissable... */
} block_t;

map_t mapFromFile(char * fileName);
void freeMap(map_t map);


#endif
