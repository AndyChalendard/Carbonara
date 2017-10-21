#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "map.h"
#include "charac.h"

void displayAll(SDL_Renderer * renderer, map_t map, charac_t charac, int time, int time_max);

void displayMap(SDL_Renderer * renderer, map_t map);

void displayCharac(SDL_Renderer * renderer, charac_t charac);

void displayTime(SDL_Renderer * renderer, int time, int time_max);
