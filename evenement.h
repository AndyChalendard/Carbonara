#include <SDL2/SDL.h>
#include <stdio.h>

#include "charac.h"
#include "map.h"
#include "display.h"


#ifndef __EVENT_HEADER__
#define __EVENT_HEADER__

#define SPEED_ENNEMY 6
#define SPEED_SELF   8

#define DETECT_DEPTH 4 /* ennemi compris */

typedef struct{
  int haut;
  int bas;
  int gauche;
  int droite;
}data_touche;

/* deplace le personnage */
int evenementPlay(SDL_Renderer * renderer, map_t * map, int * mapAct, int * time, data_touche * touche, charac_t * player);

void teleport(map_t map, charac_t * pc);

/* teste la collision entre deux blocks
 * x et y en px
 */
int collides(int x, int y, charac_t c);

/* une iteration de moveEnnemyTab */
void moveEnnemy(map_t map, int i);
void moveEnnemyTab(map_t m);
void ennemyChangeDir(map_t map, int i);

/* ne verifie la collision que d'un cote */
int gestionCollision(map_t map, charac_t * pc, int dir);

/* teste si un ennemi detecte le joueur
 * retourne 1 si detecte
 *          0 sinon
 */
int detection(map_t map, charac_t c);

/* ne detecte que sur 3 colonnes devant un ennemi
 */
int it_detection(map_t map, int i, charac_t c);

void evenement(int * run, SDL_Event * event, data_touche * touche);

data_touche init_touche();

#endif
