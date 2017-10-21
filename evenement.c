#include "evenement.h"

data_touche init_touche()
{
  data_touche touche;
  touche.haut = 0;
  touche.bas = 0;
  touche.gauche = 0;
  touche.droite = 0;
  return touche;
}

void evenementPlay(map_t * map, data_touche * touche, charac_t * player)
{
  int last_x = player->x;
  int last_y = player->y;

  int caseX = (player->x + TAILLE_BLOC/2)/TAILLE_BLOC;
  int caseY = (player->y-50 + TAILLE_BLOC/2)/TAILLE_BLOC;

  /*Déplacement du personnage*/
  if (touche->haut == 1)
  {
    player->y -= (TAILLE_BLOC*SPEED_SELF/100);
    player->dir = DIR_UP;
    if (getBlockOnMap(map, caseX, caseY-1)->id == BLOCK_ID_WALL
          && (caseY*50) > player->y-50)
      player->y = last_y;
  }else if (touche->bas == 1){
    player->y += (TAILLE_BLOC*SPEED_SELF/100);
    player->dir = DIR_DOWN;
    if (getBlockOnMap(map, caseX, caseY+1)->id == BLOCK_ID_WALL
          && (caseY*50) < player->y-50)
      player->y = last_y;
  }else if (touche->gauche == 1){
    player->x -= (TAILLE_BLOC*SPEED_SELF/100);
    player->dir = DIR_LEFT;
    if (getBlockOnMap(map, caseX-1, caseY)->id == BLOCK_ID_WALL
          && (caseX*50 > player->x))
      player->x = last_x;
  }else if (touche->droite == 1){
    player->x += (TAILLE_BLOC*SPEED_SELF/100);
    player->dir = DIR_RIGHT;
    if (getBlockOnMap(map, caseX+1, caseY)->id == BLOCK_ID_WALL
          && (caseX)*50 < player->x)
      player->x = last_x;
  }
}


void moveEnnemy(map_t map, int i) {
   switch (map.ennemies[i].dir) {
      case DIR_LEFT:
         map.ennemies[i].x -= (TAILLE_BLOC*SPEED_ENNEMY/100);
   /*      gestionCollision(map, map.ennemies + i, DIR_LEFT);*/
         ennemyChangeDir(map, i);
         break;
      case DIR_RIGHT:
         map.ennemies[i].x += (TAILLE_BLOC*SPEED_ENNEMY/100);
      /*   gestionCollision(map, map.ennemies + i, DIR_RIGHT);*/
         ennemyChangeDir(map, i);
         break;
      case DIR_UP:
         map.ennemies[i].y -= (TAILLE_BLOC*SPEED_ENNEMY/100);
      /*   gestionCollision(map, map.ennemies + i, DIR_UP);*/
         ennemyChangeDir(map, i);
         break;
      case DIR_DOWN:
         map.ennemies[i].y += (TAILLE_BLOC*SPEED_ENNEMY/100);
      /*   gestionCollision(map, map.ennemies + i, DIR_DOWN);*/
         ennemyChangeDir(map, i);
         break;
      default:
         fprintf(stderr, "direction inconnue : %d\n", map.ennemies[i].dir);
         break;
   }
}

void ennemyChangeDir(map_t map, int i) {
   int mapX, mapY;

   if (
      map.ennemies[i].x % TAILLE_BLOC < (TAILLE_BLOC*SPEED_ENNEMY/100) &&
      map.ennemies[i].y % TAILLE_BLOC < (TAILLE_BLOC*SPEED_ENNEMY/100)
   ) {
      mapX = map.ennemies[i].x / TAILLE_BLOC;
      mapY = (map.ennemies[i].y - HAUTEUR_TEMPS) / TAILLE_BLOC;

      switch (map.map[mapX][mapY].opt) {
         case BLOCK_OPT_GO_L:
            map.ennemies[i].dir = DIR_LEFT;
            break;
         case BLOCK_OPT_GO_R:
            map.ennemies[i].dir = DIR_RIGHT;
            break;
         case BLOCK_OPT_GO_U:
            map.ennemies[i].dir = DIR_UP;
            break;
         case BLOCK_OPT_GO_D:
            map.ennemies[i].dir = DIR_DOWN;
            break;
      }
   }
}


void moveEnnemyTab(map_t m) {
   int i;

   for (i = 0; i < m.nbEnnemies; ++i) {
      moveEnnemy(m, i);
   }
}


int gestionCollision(map_t map, charac_t * pc, int dir) {
   int collision = 0;

   int mapX = pc->x / TAILLE_BLOC;
   int mapY = (pc->y - HAUTEUR_TEMPS) / TAILLE_BLOC;

   switch (dir) {
      case DIR_LEFT:
         if (map.map[mapX-1][mapY].opt == BLOCK_ID_WALL) {
            pc->x = TAILLE_BLOC * ++mapX;
            collision = 1;
         }
      case DIR_RIGHT:
         if (map.map[mapX+1][mapY].opt == BLOCK_ID_WALL) {
            pc->x = TAILLE_BLOC * mapX;
            collision = 1;
         }
      case DIR_UP:
         if (map.map[mapX][mapY-1].opt == BLOCK_ID_WALL) {
            pc->y = TAILLE_BLOC * ++mapY;
            collision = 1;
         }
      case DIR_DOWN:
         if (map.map[mapX][mapY+1].opt == BLOCK_ID_WALL) {
            pc->y = TAILLE_BLOC * mapY;
            collision = 1;
         }
   }

   return collision;
}


int detection(map_t map, charac_t c) {
   int i = 0, res = 0;

   while (i < map.nbEnnemies && !res) {
      res = it_detection(map, i, c);  ++i;
   }

   return res;
}


int it_detection(map_t map, int k, charac_t c) {
   int res = 0;
   int i = 0;

   int cX = c.x / TAILLE_BLOC;
   int cY = (c.y - HAUTEUR_TEMPS) / TAILLE_BLOC;

   int eX = map.ennemies[k].x / TAILLE_BLOC;
   int eY = (map.ennemies[k].y - HAUTEUR_TEMPS) / TAILLE_BLOC;

   switch (map.ennemies[k].dir) {
      case DIR_LEFT:
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX-i][eY].id != BLOCK_ID_WALL) {
            res = (cX == eX-i) && (cY == eY);
            ++i;
         }
         break;
      case DIR_RIGHT:
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX+i][eY].id != BLOCK_ID_WALL) {
            res = (cX == eX+i) && (cY == eY);
            ++i;
         }
         break;
      case DIR_UP:
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX][eY-i].id != BLOCK_ID_WALL) {
            res = (cX == eX) && (cY == eY-i);
            ++i;
         }
         break;
      case DIR_DOWN:
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX][eY+i].id != BLOCK_ID_WALL) {
            res = (cX == eX) && (cY == eY+i);
            ++i;
         }
         break;
      default:
         fprintf(stderr, "direction inconnue : %d\n", map.ennemies[k].dir);
         break;

   }

   return res || (cX == eX && cY == eY);
}


void evenement(int * run, SDL_Event * event, data_touche * touche)
{
  while(SDL_PollEvent(event)){
      switch(event->type){
      /*case SDL_MOUSEBUTTONDOWN:
          if(event.button.x < 100 && event.button.y < 40 && event.button.x > 0 && event.button.y > 0)
              printf("PIXEL\n");
          break;*/
      case SDL_KEYDOWN:
          switch(event->key.keysym.sym){
          case SDLK_UP:
              touche->haut=1;
              break;
          case SDLK_DOWN:
              touche->bas=1;
              break;
          case SDLK_LEFT:
              touche->gauche=1;
              break;
          case SDLK_RIGHT:
              touche->droite=1;
              break;
          }
          break;
      case SDL_KEYUP:
          switch(event->key.keysym.sym){
          case SDLK_UP:
              touche->haut=0;
              break;
          case SDLK_DOWN:
              touche->bas=0;
              break;
          case SDLK_LEFT:
              touche->gauche=0;
              break;
          case SDLK_RIGHT:
              touche->droite=0;
              break;
          }
          break;
      case SDL_QUIT:
          *run = 0;
          break;
      }
   }
}
