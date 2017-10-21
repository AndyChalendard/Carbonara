#include <stdlib.h>

#ifndef __CHARAC_HEADER__
#define __CHARAC_HEADER__

#define DIR_STOP  1000
#define DIR_LEFT  1001
#define DIR_RIGHT 1002
#define DIR_UP    1003
#define DIR_DOWN  1004


typedef struct {
   int x;
   int y;
   int dir;
} charac_t;


charac_t new_charac(int x, int y); /* depart DIR_STOP */

#endif
