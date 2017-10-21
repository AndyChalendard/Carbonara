#include "evenement.h"
#include "charac.h"

data_touche init_touche()
{
  data_touche touche;
  touche.haut = 0;
  touche.bas = 0;
  touche.gauche = 0;
  touche.droite = 0;
  return touche;
}

void evenementPlay(data_touche * touche, charac_t * player)
{
  if (touche->haut == 1)
    player->x -= 2;
  if (touche->bas == 1)
    player->x += 2;
  if (touche->gauche == 1)
    player->y -= 2;
  if (touche->droite ==1)
    player->y += 2;

  if (player->x < 0)
    player->x = 0;
  if (player->x > LargeurFenetre)
    player->x = LargeurFenetre;
  if (player->y < 50)
    player->y = 50;
  if (player->y > HauteurFenetre)
    player->y = HauteurFenetre;
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
