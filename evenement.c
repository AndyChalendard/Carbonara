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
  /*Déplacement du personnage*/
  if (touche->haut == 1)
    player->y -= TAILLE_BLOC/25;
  if (touche->bas == 1)
    player->y += TAILLE_BLOC/25;
  if (touche->gauche == 1)
    player->x -= TAILLE_BLOC/25;
  if (touche->droite == 1)
    player->x += TAILLE_BLOC/25;

}


void moveEnnemy(map_t map, int i) {
   switch (map.ennemies[i].dir) {
      case DIR_LEFT:
         map.ennemies[i].x -= SPEED_ENNEMY;
   /*      gestionCollision(map, map.ennemies + i, DIR_LEFT);*/
         ennemyChangeDir(map, i);
         break;
      case DIR_RIGHT:
         map.ennemies[i].x += SPEED_ENNEMY;
      /*   gestionCollision(map, map.ennemies + i, DIR_RIGHT);*/
         ennemyChangeDir(map, i);
         break;
      case DIR_UP:
         map.ennemies[i].y -= SPEED_ENNEMY;
      /*   gestionCollision(map, map.ennemies + i, DIR_UP);*/
         ennemyChangeDir(map, i);
         break;
      case DIR_DOWN:
         map.ennemies[i].y += SPEED_ENNEMY;
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
      map.ennemies[i].x % TAILLE_BLOC < SPEED_ENNEMY &&
      map.ennemies[i].y % TAILLE_BLOC < SPEED_ENNEMY
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
