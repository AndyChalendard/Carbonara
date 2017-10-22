#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef __MENU_HEADER__
#define __MENU_HEADER__


typedef struct {
   SDL_Texture  * t;
   SDL_Renderer * rdr;
} menu_t;

/* menu screen --> renderer */

menu_t createMenu(SDL_Renderer * rdr, char * picturePath);

/* affiche le menu tant aue les events autorises ne sont pas effectues
 * retourne 1 si le joueur veut jouer   | clic sur "jouer"
 *          0 sinon                     | clic sur "quitter"
 */
int playMenu(menu_t menu);

void freeMenu(menu_t menu);




#endif
