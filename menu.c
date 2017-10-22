#include "menu.h"

menu_t createMenu(SDL_Renderer * rdr, char * picturePath) {
   SDL_Surface * s = NULL;
   menu_t menu;

   menu.rdr = rdr;

   s=IMG_Load(picturePath);
   if(s!=NULL){
     menu.t = SDL_CreateTextureFromSurface(menu.rdr,s);

     SDL_FreeSurface(s);
  } else {
     fprintf(stderr, "erreur chargement %s\n", picturePath);
 }

   return menu;
}


int playMenu(menu_t menu) {
   SDL_Event event;
   int run = 2;
   SDL_Rect rect, imgRect;

   int cpt = 0;

   imgRect.x = 0;
   imgRect.y = 0;
   imgRect.w = 950;
   imgRect.h = 600;

   rect.w = 250;
   rect.h = 68;
   rect.x = 352;

   printf("%d\n", ++cpt);
   while (run == 2) {
      while (SDL_PollEvent(&event)) {
         switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
               if (
                  event.button.x >= 352 && event.button.y >= 365 &&
                  event.button.x <  602 && event.button.y <  433
               ) {
                  run = 1;
               } else if (
                  event.button.x >= 353 && event.button.y >= 466 &&
                  event.button.x <  603 && event.button.y <  534
               ) {
                  run = 0;
               }
               break;
            case SDL_MOUSEMOTION:
               if (
                  event.button.x >= 352 && event.button.y >= 365 &&
                  event.button.x <  602 && event.button.y <  433
               ) {
                  rect.y = 365;
                  SDL_SetRenderDrawColor(menu.rdr, 240, 30, 30, 0);
                  SDL_RenderDrawRect(menu.rdr, &rect);
               } else if (
                  event.button.x >= 353 && event.button.y >= 466 &&
                  event.button.x <  603 && event.button.y <  534
               ) {
                  rect.y = 466;
                  SDL_SetRenderDrawColor(menu.rdr, 240, 30, 30, 0);
                  SDL_RenderDrawRect(menu.rdr, &rect);
               } else {
                  SDL_RenderCopy(menu.rdr,menu.t,NULL,&imgRect);
               }
               break;
         }
      }

      SDL_RenderPresent(menu.rdr);

      SDL_Delay(32);
   }

   return run;
}


void freeMenu(menu_t menu) {
     SDL_DestroyTexture(menu.t);
     SDL_DestroyRenderer(menu.rdr);
}
