#include "charac.h"

charac_t new_charac(int x, int y, int dir) {
   charac_t c;

   c.x = x;
   c.y = y;
   c.dir = dir;
   c.t_up = NULL;
   c.t_down = NULL;
   c.t_right = NULL;
   c.t_left = NULL;

   return c;
}
