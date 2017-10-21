#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "charac.h"

#ifndef __MAP_HEADER__
#define __MAP_HEADER__


// liste des dir -> charac.h

/* LISTE DES IDs */
#define BLOCK_ID_WALL 2000
#define BLOCK_ID_GRND 2001
#define BLOCK_ID_STRT 2002
#define BLOCK_ID_END  2003

/* LISTE DES OPTIONS */
#define BLOCK_OPT_GO_L 3001
#define BLOCK_OPT_GO_R 3002
#define BLOCK_OPT_GO_U 3003
#define BLOCK_OPT_GO_D 3004

/* CODES FICHIER */
#define FIC_CODE_m 4001
#define FIC_CODE_s 4002

#define FIC_CODE_T 4003
#define FIC_CODE_A 4004

#define FIC_CODE_G 4005
#define FIC_CODE_D 4006
#define FIC_CODE_H 4007
#define FIC_CODE_B 4008

#define FIC_CODE_g 4009
#define FIC_CODE_d 4010
#define FIC_CODE_h 4011
#define FIC_CODE_b 4012


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


/* x, y = position de depart du perso
 */
map_t mapFromFile(char * fileName, int x, int y);
void  freeMap(map_t map);

/* convertit un texte avec des codes lettres (m/s/D/...) en codes nombres
 */
void convertFile(char * fileName, char * newFile);

block_t new_block(int id, int opt);


#endif
