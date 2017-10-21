#include <SDL2/SDL.h>

#include "charac.h"
#include "map.h"
#include "display.h"


#ifndef __EVENT_HEADER__
#define __EVENT_HEADER__

typedef struct{
  int haut;
  int bas;
  int gauche;
  int droite;
}data_touche;

void evenementPlay(map_t * map, data_touche * touche, charac_t * player);

void evenement(int * run, SDL_Event * event, data_touche * touche);

data_touche init_touche();


#endif
