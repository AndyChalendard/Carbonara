#include "evenement.h"

data_touche init_touche()
{
  data_touche touche;
  touche.haut = 0;
  touche.bas = 0;
  touche.gauche = 0;
  touche.droite = 0;
  touche.space = 0;
  return touche;
}

int evenementPlay(SDL_Renderer * renderer, int * pause, map_t * map, int * mapAct, int * time, data_touche * touche, charac_t * player)
{
  int last_x = player->x;
  int last_y = player->y;

  int caseX = (player->x + TAILLE_BLOC/2)/TAILLE_BLOC;
  int caseY = (player->y-HAUTEUR_TEMPS + TAILLE_BLOC/2)/TAILLE_BLOC;

  /*block_t * block = getBlockOnMap(map, caseX, caseY-1);*/

  int decallage = 13;

  if (*pause == 1)
  {
    return 0;
  }

  if ((caseX*TAILLE_BLOC) + decallage > player->x && (caseX*TAILLE_BLOC) - decallage < player->x)
  {
    if ((caseY*TAILLE_BLOC) + decallage > player->y-50 && (caseY*TAILLE_BLOC) - decallage < player->y-50)
    {
      switch (map->map[caseX][caseY].opt) {
         case BLOCK_OPT_TP_Q:
            teleport(*map, player);
            break;
         case BLOCK_OPT_END:
            *(mapAct) = *(mapAct) + 1;
            *time = 0;
            if (reloadGame(renderer, *mapAct, map, player))
            {
              IMG_Quit();
              TTF_Quit();
              SDL_Quit();
              return 1;
            }
            break;
        case BLOCK_OPT_PPR:
          *(mapAct) = 7;
          map->map[caseX][caseY].opt = BLOCK_ID_GRND;
          break;
      }
    }
  }

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

  return 0;
}

void teleport(map_t map, charac_t * pc) {
   int caseX = (pc->x + TAILLE_BLOC/2)/TAILLE_BLOC;
   int caseY = (pc->y-HAUTEUR_TEMPS + TAILLE_BLOC/2)/TAILLE_BLOC;

   if (map.map[caseX][caseY].opt == BLOCK_OPT_TP_Q) {
      pc->x = TAILLE_BLOC * map.map[caseX][caseY].opt_data->v1;
      pc->y = TAILLE_BLOC * map.map[caseX][caseY].opt_data->v2 + HAUTEUR_TEMPS;
   }
}


int collides(int x, int y, charac_t c) {
   return (
      c.x > x + TAILLE_BLOC ||
      c.y > y + TAILLE_BLOC ||
      x > c.x + TAILLE_BLOC ||
      y > c.y + TAILLE_BLOC
   );
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

   int cX = (c.x+25) / TAILLE_BLOC;
   int cY = ((c.y+25) - HAUTEUR_TEMPS) / TAILLE_BLOC;

   int eX = map.ennemies[k].x / TAILLE_BLOC;
   int eY = (map.ennemies[k].y - HAUTEUR_TEMPS) / TAILLE_BLOC;

   switch (map.ennemies[k].dir) {
      case DIR_LEFT:
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX-i][eY].id != BLOCK_ID_WALL) {
            res = (cX == eX-i) && (cY == eY);
            ++i;
         }
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX-i][eY-1].id != BLOCK_ID_WALL) {
            res = (cX == eX-i) && (cY == eY-1);
            ++i;
         }
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX-i][eY+1].id != BLOCK_ID_WALL) {
            res = (cX == eX-i) && (cY == eY+1);
            ++i;
         }
         break;
      case DIR_RIGHT:
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX+i][eY].id != BLOCK_ID_WALL) {
            res = (cX == eX+i) && (cY == eY);
            ++i;
         }
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX+i][eY-1].id != BLOCK_ID_WALL) {
            res = (cX == eX+i) && (cY == eY-1);
            ++i;
         }
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX+i][eY+1].id != BLOCK_ID_WALL) {
            res = (cX == eX+i) && (cY == eY+1);
            ++i;
         }
         break;
      case DIR_UP:
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX][eY-i].id != BLOCK_ID_WALL) {
            res = (cX == eX) && (cY == eY-i);
            ++i;
         }
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX-1][eY-i].id != BLOCK_ID_WALL) {
            res = (cX == eX-1) && (cY == eY-i);
            ++i;
         }
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX+1][eY-i].id != BLOCK_ID_WALL) {
            res = (cX == eX+1) && (cY == eY-i);
            ++i;
         }
         break;
      case DIR_DOWN:
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX][eY+i].id != BLOCK_ID_WALL) {
            res = (cX == eX) && (cY == eY+i);
            ++i;
         }
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX-1][eY+i].id != BLOCK_ID_WALL) {
            res = (cX == eX-1) && (cY == eY+i);
            ++i;
         }
         i = 1;
         while (!res && i < DETECT_DEPTH && map.map[eX+1][eY+i].id != BLOCK_ID_WALL) {
            res = (cX == eX+1) && (cY == eY+i);
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
  touche->space=0;
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
          case SDLK_SPACE:
              touche->space=1;
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
