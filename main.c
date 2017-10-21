#include <stdio.h>
#include <SDL2/SDL_ttf.h>

#include "evenement.h"
#include "display.h"
#include "charac.h"
#include "map.h"

int main()
{
  int time = 0;
  int timeMax = 3600;
  data_touche touche;
  int mapAct = 1;
  int pause = 1;

  charac_t player;
  SDL_Renderer * renderer;

  TTF_Font * font;

  map_t map;

  /* variable d'initialisation de SDL_image */
  int flags = IMG_INIT_JPG | IMG_INIT_PNG;
  /* variable liee a la fenetre */
  SDL_Window * window;
  /* variables liees a la capture d'evenement */
  SDL_Event event;
  int run = 1;

  printf("Variables initialisées !\n");

  /* initialisation de la SDL2 */
  if(SDL_Init(SDL_INIT_VIDEO) == 1){
      fprintf(stderr,"Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
      return EXIT_FAILURE;
  }

  /* initialisation de SDL_ttf */
  if (TTF_Init() != 0){
      fprintf(stderr, "Erreur d'initialisation TTF : %s\n", TTF_GetError());
      SDL_Quit();
      return EXIT_FAILURE;
  }

  /* initialisation de SDL_image */
  if((IMG_Init(flags)&flags) != flags){
      printf("IMG_Init: Failed to init required jpg and png support!\n");
      printf("IMG_Init: %s\n", IMG_GetError());
      TTF_Quit();
      SDL_Quit();
      return EXIT_FAILURE;
  }

  /* ouverture de la fenetre */
  window = SDL_CreateWindow("Carbonara",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            LARGEUR_FENETRE,HAUTEUR_FENETRE,
                            0);

  if(!window){
    fprintf(stderr,"Erreur de creation de la fenetre : %s\n", SDL_GetError());
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return EXIT_FAILURE;
  }

  /* initialisation du renderer */
  renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
  if(!renderer){
    fprintf(stderr,"Erreur de creation du renderer\n");
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return EXIT_FAILURE;
  }

  printf("SDL initialisée !\n");

  /*intialisation de la map et du joueur*/
  if (loadGame(renderer, mapAct, &map, &player) == 1)
  {
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return EXIT_FAILURE;
  }

  /*initialisation de la police*/
  font = TTF_OpenFont("arial.ttf",20);

  /*initialisation des evenements clavier*/
  touche = init_touche();
  printf("Evenements initialisés !\n");


  pause = 0;
  /* boucle d'evenement */
  while(run){
    if (pause == 0)
      time += 1;
    evenement(&run, &event, &touche);
    if (evenementPlay(renderer, &pause, &map, &mapAct, &time, &touche, &player) == 1)
    {
      IMG_Quit();
      TTF_Quit();
      SDL_Quit();
      return EXIT_FAILURE;
    }

    if (pause==0)
    {
      moveEnnemyTab(map);

      if (detection(map, player)) {
        printf("detection\n");
      }
    }

    displayAll(renderer, &pause, font, map, player, time, timeMax);
    SDL_Delay(32);
    if (time > timeMax)
    {
      time = 0;
      if (reloadGame(renderer, mapAct, &map, &player))
      {
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
      }
    }
  }

  printf("Déchargement du jeu...\n");

  closeTexture(&map, &player);

  /* on libere le renderer */
  if(renderer)
    SDL_DestroyRenderer(renderer);

  /* on libere la fenetre */
  SDL_DestroyWindow(window);
  /* fermeture de SDL_image, SDL_ttf et SDL2 */
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();

  return 0;
}
