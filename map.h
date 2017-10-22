#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "charac.h"

#ifndef __MAP_HEADER__
#define __MAP_HEADER__


/* liste des dir -> charac.h */

/* LISTE DES IDs */
#define BLOCK_ID_WALL 2000
#define BLOCK_ID_GRND 2001

/* LISTE DES OPTIONS */
#define BLOCK_OPT_STRT 3000

#define BLOCK_OPT_GO_L 3001
#define BLOCK_OPT_GO_R 3002
#define BLOCK_OPT_GO_U 3003
#define BLOCK_OPT_GO_D 3004

#define BLOCK_OPT_END  3005

#define BLOCK_OPT_TP_Q 3006
#define BLOCK_OPT_TP_q 3007

#define BLOCK_OPT_PPR  3008


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

#define FIC_CODE_Q 4013
#define FIC_CODE_q 4014

#define FIC_CODE_P 4015


typedef struct {
   int v1;
   int v2;
   SDL_Texture * t;
} block_opt_t;

typedef struct {
   int           id;
   SDL_Texture * t;
   int           opt; /* option : franchissable... */
   block_opt_t * opt_data;
} block_t;

typedef struct {
   int         w; /* longueur */
   int         h; /* hauteur  */
   block_t **  map;
   charac_t *  ennemies;
   int         nbEnnemies;
} map_t;


/* x, y = position de depart du perso
 */
map_t mapFromFile(char * fileName, int * px, int * py);
void  freeMap(map_t map);

/* convertit un texte avec des codes lettres (m/s/D/...) en codes nombres
 */
void convertFile(char * fileName, char * newFile);

block_t new_block(int id, int opt, block_opt_t * data);

block_opt_t * new_block_opt(int v1, int v2);
#endif
