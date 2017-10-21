#include "display.h"

void displayAll(SDL_Renderer * renderer, map_t map, charac_t charac, int time, int time_max)
{
  displayMap(map);
  displayCharac(charac);
  displayTime(time);
}

void displayMap(SDL_Renderer * renderer, map_t map)
{
  int i=0, j=0;
  block_t block;
  SDL_Rect rect;

  for (i=0; i<map.w; i++)
  {
    for (j=0; j<map.h; j++)
    {
      block = (block_t *) *(((block_t *) *(map.map + i)) + j);
      rect.x = i*50;
      rect.y = j*50 + 50;
      rect.w = 50;
      rect.h = 50;

      SDL_RenderCopy(renderer,block.t,NULL,&rect);
    }
  }
}

void displayCharac(SDL_Renderer * renderer, charac_t charac)
{
  SDL_Rect rect;

  rect.x = charac.x;
  rect.y = charac.y;
  rect.w = 50;
  rect.h = 50;

  SDL_RenderCopy(renderer,charac.t,NULL,&rect);
}

void displayTime(SDL_Renderer * renderer, int time, int time_max)
{
  SDL_Rect rect;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  rect.x = 0;
  rect.y = 0;
  rect.w = LargeurFenetre;
  rect.h = 50;
  SDL_RenderFillRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
  rect.x = 10;
  rect.y = 10;
  rect.w = (time/time_max)*LargeurFenetre-20;
  rect.h = 30;
  SDL_RenderFillRect(renderer, &rect);
}
