#include <SDL2/SDL.h>
#include <stdio.h>

#include "charac.h"
#include "map.h"
#include "display.h"


#ifndef __EVENT_HEADER__
#define __EVENT_HEADER__

#define SPEED_ENNEMY 1
#define SPEED_SELF   2

#define DETECT_DEPTH 4 /* ennemi compris */

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
