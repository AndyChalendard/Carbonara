#include "display.h"

void displayAll(SDL_Renderer * renderer, int * pause, TTF_Font * font, map_t map, charac_t charac, int time, int time_max)
{
  displayMap(renderer, map);
  displayEnnemies(renderer, map);
  displayCharac(renderer, charac);
  displayTime(renderer, time, time_max);

  if (*pause == 1)
  {
    displayPause(renderer, font);
  }

  displayVision(renderer, map);
  /* finalisation de l'affichage */
  SDL_RenderPresent(renderer);
}

void displayPause(SDL_Renderer * renderer, TTF_Font * font)
{
  SDL_Rect rect;
  SDL_Color noir = {0,0,255,0};
  SDL_Texture  * t;
  char txt[] = "PAUSE";

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
  rect.x = 70;
  rect.y = 70;
  rect.w = LARGEUR_FENETRE-100;
  rect.h = 50;
  SDL_RenderFillRect(renderer, &rect);

  t = NULL;
  img_text(renderer, font, t, txt, noir, rect);
}

void displayMap(SDL_Renderer * renderer, map_t map)
{
  int i=0, j=0;
  block_t * block;
  SDL_Rect rect;
  SDL_Surface * s = NULL;
  SDL_Texture * t;

  for (i=0; i<map.w; i++)
  {
    for (j=0; j<map.h; j++)
    {
      block = getBlockOnMap(&map, i, j);
      rect.x = i*TAILLE_BLOC;
      rect.y = j*TAILLE_BLOC + 50;
      rect.w = TAILLE_BLOC;
      rect.h = TAILLE_BLOC;

      SDL_RenderCopy(renderer,block->t,NULL,&rect);

      switch (block->opt)
      {
        case BLOCK_OPT_END:
          s=IMG_Load("Textures/stairs_up.png");
          break;
        case BLOCK_OPT_PPR:
          s=IMG_Load("Textures/parcho.png");
          break;
        case BLOCK_OPT_STRT:
          s=IMG_Load("Textures/stairs_down.png");
          break;
        case BLOCK_OPT_TP_Q:
          s=IMG_Load("Textures/tpdepart.png");
          break;
        case BLOCK_OPT_TP_q:
          s=IMG_Load("Textures/tparrivee.png");
          break;
      }
      if(s!=NULL){
        t = SDL_CreateTextureFromSurface(renderer,s);
        SDL_RenderCopy(renderer,t,NULL,&rect);

        SDL_FreeSurface(s);
        if(t)
          SDL_DestroyTexture(t);
      }
      s=NULL;
    }
  }
}

void displayEnnemies(SDL_Renderer * renderer, map_t map)
{
  int i;

  for (i = 0; i<map.nbEnnemies; i++)
  {
    displayCharac(renderer, *(map.ennemies + i));
  }
}

void displayCharac(SDL_Renderer * renderer, charac_t charac)
{
  SDL_Rect rect;

  rect.x = charac.x;
  rect.y = charac.y;
  rect.w = TAILLE_BLOC;
  rect.h = TAILLE_BLOC;
  switch (charac.dir)
  {
    case DIR_STOP:
    case DIR_DOWN:
      SDL_RenderCopy(renderer,charac.t_down,NULL,&rect);
      break;
    case DIR_UP:
      SDL_RenderCopy(renderer,charac.t_up,NULL,&rect);
      break;
    case DIR_RIGHT:
      SDL_RenderCopy(renderer,charac.t_right,NULL,&rect);
      break;
    case DIR_LEFT:
      SDL_RenderCopy(renderer,charac.t_left,NULL,&rect);
      break;
  }
}

void displayTime(SDL_Renderer * renderer, int time, int time_max)
{
  SDL_Rect rect;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  rect.x = 0;
  rect.y = 0;
  rect.w = LARGEUR_FENETRE;
  rect.h = 50;
  SDL_RenderFillRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
  rect.x = 10;
  rect.y = 10;
  rect.w = (LARGEUR_FENETRE-20) - (time*(LARGEUR_FENETRE-20))/time_max;
  rect.h = 30;
  SDL_RenderFillRect(renderer, &rect);
}

void img_text(SDL_Renderer * renderer, TTF_Font * font, SDL_Texture  * t, char * text, SDL_Color couleur, SDL_Rect rect)
{
  SDL_Surface * s;
  s = TTF_RenderText_Blended(font,text,couleur);
  if(s){
      t = SDL_CreateTextureFromSurface(renderer,s);
      SDL_FreeSurface(s);
      SDL_RenderCopy(renderer, t, NULL, &rect);
  }
}

int loadGame(SDL_Renderer * renderer, int level, map_t * map, charac_t * player)
{
  int x_init_player = 0;
  int y_init_player = 0;

  char fileMap[16];
  sprintf(fileMap, "Data/Map/etage%d", level);
  printf("Chargement de la map: %s...\n", fileMap);
  *map = mapFromFile(fileMap, &x_init_player, &y_init_player);
  initMapTexture(renderer, map);
  if (map->w != 0 && map->h != 0)
  {
    printf("MAP initialisée !\n");
    printf("La map fait %d*%d et contient:%d ennemies \n", map->w, map->h, map->nbEnnemies);

    /*initialisation des différentes variables*/
    initEnnemis(renderer, map);
    *player = new_charac(x_init_player*TAILLE_BLOC, y_init_player*TAILLE_BLOC+50, DIR_STOP);
    initPlayerTexture(renderer, player);
    printf("Ennemies et joueur initialisé !\n");
  }else{
    fprintf(stderr,"Erreur lors de la création de la MAP\n");
    printf("La map fait %d*%d et contient:%d ennemies \n", map->w, map->h, map->nbEnnemies);

    freeMap(*map);
    return 1;
  }

  return 0;
}

void displayVision(SDL_Renderer * renderer, map_t map) {
   int i, k;
   int eX, eY;

   SDL_Rect rect;
   SDL_Surface * s;
   SDL_Texture * t;

   rect.w = TAILLE_BLOC;
   rect.h = TAILLE_BLOC;

   s=IMG_Load("Textures/vision.png");
   if(s!=NULL){
     t = SDL_CreateTextureFromSurface(renderer,s);

     SDL_FreeSurface(s);
   }

   for (k = 0; k < map.nbEnnemies; ++k) {
      eX = map.ennemies[k].x / TAILLE_BLOC;
      eY = (map.ennemies[k].y - HAUTEUR_TEMPS) / TAILLE_BLOC;

      switch (map.ennemies[k].dir) {
         case DIR_LEFT:
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX-i][eY].id != BLOCK_ID_WALL) {
              rect.x = (eX-i)*TAILLE_BLOC;
              rect.y = eY*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX-i][eY-1].id != BLOCK_ID_WALL) {
              rect.x = eX-i*TAILLE_BLOC;
              rect.y = (eY-1)*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX-i][eY+1].id != BLOCK_ID_WALL) {
              rect.x = eX-i*TAILLE_BLOC;
              rect.y = eY+1*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            break;
         case DIR_RIGHT:
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX+i][eY].id != BLOCK_ID_WALL) {
              rect.x = (eX+i)*TAILLE_BLOC;
              rect.y = eY*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX+i][eY-1].id != BLOCK_ID_WALL) {
              rect.x = (eX+i)*TAILLE_BLOC;
              rect.y = (eY-1)*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX+i][eY+1].id != BLOCK_ID_WALL) {
              rect.x = (eX+i)*TAILLE_BLOC;
              rect.y = (eY+1)*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            break;
         case DIR_UP:
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX][eY-i].id != BLOCK_ID_WALL) {
              rect.x = (eX)*TAILLE_BLOC;
              rect.y = (eY-i)*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX-1][eY-i].id != BLOCK_ID_WALL) {
              rect.x = (eX-1)*TAILLE_BLOC;
              rect.y = (eY-i)*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX+1][eY-i].id != BLOCK_ID_WALL) {
              rect.x = (eX+1)*TAILLE_BLOC;
              rect.y = (eY-i)*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            break;
         case DIR_DOWN:
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX][eY+i].id != BLOCK_ID_WALL) {
              rect.x = (eX)*TAILLE_BLOC;
              rect.y = (eY+i)*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX-1][eY+i].id != BLOCK_ID_WALL) {
              rect.x = (eX-1)*TAILLE_BLOC;
              rect.y = (eY+i)*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            i = 1;
            while (i < DETECT_DEPTH && map.map[eX+1][eY+i].id != BLOCK_ID_WALL) {
              rect.x = (eX+1)*TAILLE_BLOC;
              rect.y = (eY+i)*TAILLE_BLOC + 50;
              SDL_RenderCopy(renderer,t,NULL,&rect);
               ++i;
            }
            break;
      }
   }

   if(t)
     SDL_DestroyTexture(t);
}


int reloadGame(SDL_Renderer * renderer, int level, map_t * map, charac_t * player)
{
  freeMap(*map);

  return loadGame(renderer, level, map, player);;
}

block_t * getBlockOnMap(map_t * map, int X, int Y)
{
  return map->map[X] + Y;
}

void initEnnemis(SDL_Renderer * renderer, map_t * map)
{
  charac_t * ennemie;
  int i;
  SDL_Surface * s;

  for (i = 0; i<map->nbEnnemies; i++)
  {
    ennemie = map->ennemies + i;
    ennemie->x = ennemie->x*TAILLE_BLOC;
    ennemie->y = ennemie->y*TAILLE_BLOC + 50;

    s=IMG_Load("Textures/ennemie_droite.png");
    if(s){
      ennemie->t_right = SDL_CreateTextureFromSurface(renderer,s);
      SDL_FreeSurface(s);
    }
    s=IMG_Load("Textures/ennemie_gauche.png");
    if(s){
      ennemie->t_left = SDL_CreateTextureFromSurface(renderer,s);
      SDL_FreeSurface(s);
    }
    s=IMG_Load("Textures/ennemie_haut.png");
    if(s){
      ennemie->t_up = SDL_CreateTextureFromSurface(renderer,s);
      SDL_FreeSurface(s);
    }
    s=IMG_Load("Textures/ennemie_bas.png");
    if(s){
      ennemie->t_down = SDL_CreateTextureFromSurface(renderer,s);
      SDL_FreeSurface(s);
    }
  }
}

void initPlayerTexture(SDL_Renderer * renderer, charac_t * player)
{
  SDL_Surface * s;

  s=IMG_Load("Textures/player_droite.png");
  if(s){
    player->t_right = SDL_CreateTextureFromSurface(renderer,s);
    SDL_FreeSurface(s);
  }
  s=IMG_Load("Textures/player_gauche.png");
  if(s){
    player->t_left = SDL_CreateTextureFromSurface(renderer,s);
    SDL_FreeSurface(s);
  }
  s=IMG_Load("Textures/player_haut.png");
  if(s){
    player->t_up = SDL_CreateTextureFromSurface(renderer,s);
    SDL_FreeSurface(s);
  }
  s=IMG_Load("Textures/player_bas.png");
  if(s){
    player->t_down = SDL_CreateTextureFromSurface(renderer,s);
    SDL_FreeSurface(s);
  }
}

void initMapTexture(SDL_Renderer * renderer, map_t * map)
{
  int i=0, j=0;
  block_t * block;
  SDL_Surface * s;
  SDL_Texture * mur;
  SDL_Texture * sol;

  s=IMG_Load("Textures/sol.png");
  if(s){
    sol = SDL_CreateTextureFromSurface(renderer,s);
    SDL_FreeSurface(s);
  }

  s=IMG_Load("Textures/mur.png");
  if(s){
    mur = SDL_CreateTextureFromSurface(renderer,s);
    SDL_FreeSurface(s);
  }

  for (i=0; i<map->w; i++)
  {
    for (j=0; j<map->h; j++)
    {
      block = getBlockOnMap(map, i, j);
      switch (block->id)
      {
        case BLOCK_ID_WALL:
          block->t = mur;
          break;
        case BLOCK_ID_GRND:
          block->t = sol;
          break;
      }
    }
  }
}

void closeMapTexture(map_t * map)
{
  int i=0, j=0;
  block_t * block;

  for (i=0; i<map->w; i++)
  {
    for (j=0; j<map->h; j++)
    {
      block = getBlockOnMap(map, i, j);
      if(block->t)
        SDL_DestroyTexture(block->t);
    }
  }
}

void closePlayer(charac_t * player)
{
  if(player->t_down)
    SDL_DestroyTexture(player->t_down);
  if(player->t_up)
    SDL_DestroyTexture(player->t_up);
  if(player->t_right)
    SDL_DestroyTexture(player->t_right);
  if(player->t_left)
    SDL_DestroyTexture(player->t_left);
}

void closeEnnemies(map_t * map)
{
  int i;

  for (i = 0; i<map->nbEnnemies; i++)
  {
    closePlayer(map->ennemies + i);
  }
}

void closeTexture(map_t * map, charac_t * player)
{
  closeEnnemies(map);
  closePlayer(player);
  closeMapTexture(map);
}
