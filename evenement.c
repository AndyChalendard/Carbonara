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
  block_t * blockDroite;
  block_t * blockGauche;
  block_t * blockBas;
  block_t * blockHaut;

  int caseX, caseY;

  /*Déplacement du personnage*/
  if (touche->haut == 1)
    player->x -= 2;
  if (touche->bas == 1)
    player->x += 2;
  if (touche->gauche == 1)
    player->y -= 2;
  if (touche->droite == 1)
    player->y += 2;

  /*block autour*/
  caseX = (player->x)/TAILLE_BLOC;
  caseY = (player->y-50)/TAILLE_BLOC;

  blockDroite = getBlockOnMap(map, caseX+1, caseY);
  blockGauche = getBlockOnMap(map, caseX-1, caseY);
  blockBas = getBlockOnMap(map, caseX, caseY+1);
  blockHaut = getBlockOnMap(map, caseX, caseY-1);

  if (blockDroite->opt == BLOCK_ID_WALL && player->x > caseX * TAILLE_BLOC)
    player->x = caseX * TAILLE_BLOC;
  if (blockGauche->opt == BLOCK_ID_WALL && player->x < caseX * TAILLE_BLOC)
    player->x = caseX * TAILLE_BLOC;
  if (blockBas->opt == BLOCK_ID_WALL && player->y > (caseY+1) * TAILLE_BLOC)
    player->y = caseY * TAILLE_BLOC + 50;
  if (blockHaut->opt == BLOCK_ID_WALL && player->y < (caseY+1) * TAILLE_BLOC)
    player->y = caseY * TAILLE_BLOC + 50;
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
