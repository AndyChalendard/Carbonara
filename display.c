#include "display.h"

void displayAll(SDL_Renderer * renderer, map_t map, charac_t charac, int time, int time_max)
{
  displayMap(renderer, map);
  displayEnnemies(renderer, map);
  displayCharac(renderer, charac);
  displayTime(renderer, time, time_max);

  /* finalisation de l'affichage */
  SDL_RenderPresent(renderer);
}

void displayMap(SDL_Renderer * renderer, map_t map)
{
  int i=0, j=0;
  block_t * block;
  SDL_Rect rect;

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

int loadGame(SDL_Renderer * renderer, char fileMap[], map_t * map, charac_t * player)
{
  int x_init_player = 0;
  int y_init_player = 0;

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

int reloadGame(SDL_Renderer * renderer, char fileMap[], map_t * map, charac_t * player)
{
  freeMap(*map);

  return loadGame(renderer, fileMap, map, player);;
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
