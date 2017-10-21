#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

void displayAll(SDL_Renderer * renderer, map_t map, chara_t charac, int time, int time_max);

void displayMap(SDL_Renderer * renderer, map_t map);

void displayCharac(SDL_Renderer * renderer, charac_t charac);

void displayTime(SDL_Renderer * renderer, int time, int time_max);
