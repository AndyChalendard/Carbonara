#include <SDL2/SDL.h>
#include <stdio.h>

#include "charac.h"
#include "map.h"
#include "display.h"


#ifndef __EVENT_HEADER__
#define __EVENT_HEADER__

#define SPEED_ENNEMY 1

typedef struct{
  int haut;
  int bas;
  int gauche;
  int droite;
}data_touche;

/* deplace le personnage */
void evenementPlay(map_t * map, data_touche * touche, charac_t * player);

/* une iteration de moveEnnemyTab */
void moveEnnemy(map_t map, int i);
void moveEnnemyTab(map_t m);

/* ne verifie la collision que d'un cote */
int gestionCollision(map_t m, charac_t c, int dir);

void evenement(int * run, SDL_Event * event, data_touche * touche);

data_touche init_touche();


#endif
