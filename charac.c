#include "charac.h"

charac_t new_charac(int x, int y, int dir) {
   charac_t c;

   c.x = x;
   c.y = y;
   c.dir = dir;
   c.t = NULL;

   return c;
}
