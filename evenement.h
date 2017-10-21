#include <SDL2/SDL.h>

#include "charac.h"

typedef struct{
  int haut;
  int bas;
  int gauche;
  int droite;
}data_touche;

void evenementPlay(data_touche * touche, charac_t * player);

void evenement(int * run, SDL_Event * event, data_touche * touche);

data_touche init_touche();
