#include "display.h"

void displayAll(SDL_Renderer * renderer, map_t map, charac_t charac, int time, int time_max)
{
  displayMap(renderer, map);
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

void displayCharac(SDL_Renderer * renderer, charac_t charac)
{
  SDL_Rect rect;

  rect.x = charac.x;
  rect.y = charac.y;
  rect.w = TAILLE_BLOC;
  rect.h = TAILLE_BLOC;

  SDL_RenderCopy(renderer,charac.t,NULL,&rect);
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
  rect.w = (time/time_max)*LARGEUR_FENETRE-20;
  rect.h = 30;
  SDL_RenderFillRect(renderer, &rect);
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

    s=IMG_Load("Textures/ennemie.png");
    if(s){
      ennemie->t = SDL_CreateTextureFromSurface(renderer,s);
      SDL_FreeSurface(s);
    }
  }
}

void initPlayerTexture(SDL_Renderer * renderer, charac_t * player)
{
  SDL_Surface * s;

  s=IMG_Load("Textures/player.png");
  if(s){
    player->t = SDL_CreateTextureFromSurface(renderer,s);
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
        case BLOCK_ID_END:
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
  if(player->t)
    SDL_DestroyTexture(player->t);
}

void closeEnnemies(map_t * map)
{
  charac_t * ennemie;
  int i;

  for (i = 0; i<map->nbEnnemies; i++)
  {
    ennemie = map->ennemies + i;

    if(ennemie->t)
      SDL_DestroyTexture(ennemie->t);
  }
}

void closeTexture(map_t * map, charac_t * player)
{
  closeEnnemies(map);
  closePlayer(player);
  closeMapTexture(map);
}
