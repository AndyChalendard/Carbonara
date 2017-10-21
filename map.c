#include "map.h"

map_t mapFromFile(char * fileName, int * px, int * py) {
   int i, j, k = 0; /* k : iterateur dans tab ennemis */
   FILE * pf = NULL;
   map_t map;
   char * newFile = ".carteBis.txt";
   int code;
   int v1, v2;

/* conversion de la carte */
   convertFile(fileName, newFile);

/* ouverture du fichier */
   pf = fopen(newFile, "r");
   if (!pf) {
      fprintf(stderr, "fichier introuvable : %s\n", newFile);
   } else {
      fscanf(pf, "%d", &(map.w));
      fscanf(pf, "%d", &(map.h));
      fscanf(pf, "%d", &(map.nbEnnemies));

/* allocation map.map */
      map.map = (block_t **) malloc(map.w * sizeof(block_t *));
      if (!map.map) {
         fprintf(stderr, "erreur allocation map.map\n");
      } else {
/* allocation map.map[i] */
         for (i = 0; i < map.w; ++i) {
            map.map[i] = (block_t *) malloc(map.h * sizeof(block_t));
            if (!map.map[i]) {
               fprintf(stderr, "erreur allocation map.map[%d]\n", i);
               /* liberer le reste... oupas */
               free(map.map);
            }
         }

         if (map.map) { /* si tous les sous-tableaux on ete liberes */
/* allocation ennemies */
            map.ennemies = (charac_t *) malloc(map.nbEnnemies * sizeof(charac_t));
            if (!map.ennemies) {
               fprintf(stderr, "erreur allocation map.ennemies");
               /* liberer tout map.map */
               free(map.map);
            } else {
/* lecture fichier */
               /* on suppose la map bien ecrite */
               for (j = 0; j < map.h; ++j) {
                  for (i = 0; i < map.w; ++i) {
                     fscanf(pf, "%d", &code);

                     switch (code) {
                        case FIC_CODE_m :
                           map.map[i][j] = new_block(BLOCK_ID_WALL, 0, NULL);
                           break;
                        case FIC_CODE_s :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, 0, NULL);
                           break;
                        case FIC_CODE_T :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, 0, NULL);
                           *px = i; *py = j;
                           break;
                        case FIC_CODE_A :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, BLOCK_OPT_END, NULL);
                           break;
                        case FIC_CODE_G :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, BLOCK_OPT_GO_L, NULL);
                           map.ennemies[k] = new_charac(i, j, DIR_LEFT); ++k;
                           break;
                        case FIC_CODE_D :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, BLOCK_OPT_GO_R, NULL);
                           map.ennemies[k] = new_charac(i, j, DIR_RIGHT); ++k;
                           break;
                        case FIC_CODE_H :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, BLOCK_OPT_GO_U, NULL);
                           map.ennemies[k] = new_charac(i, j, DIR_UP); ++k;
                           break;
                        case FIC_CODE_B :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, BLOCK_OPT_GO_D, NULL);
                           map.ennemies[k] = new_charac(i, j, DIR_DOWN); ++k;
                           break;
                        case FIC_CODE_g :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, BLOCK_OPT_GO_L, NULL);
                           break;
                        case FIC_CODE_d :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, BLOCK_OPT_GO_R, NULL);
                           break;
                        case FIC_CODE_h :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, BLOCK_OPT_GO_U, NULL);
                           break;
                        case FIC_CODE_b :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, BLOCK_OPT_GO_D, NULL);
                           break;
                        case FIC_CODE_Q :
                           fscanf(pf, "%d", &v1);
                           fscanf(pf, "%d", &v2);
                           map.map[i][j] = new_block(BLOCK_ID_GRND, BLOCK_OPT_TP_Q, new_block_opt(v1, v2));
                           break;
                        case FIC_CODE_q :
                           map.map[i][j] = new_block(BLOCK_ID_GRND, BLOCK_OPT_TP_q, NULL);
                           break;
                     }
                  }
               }


            }
         }


      }

      fclose(pf);
   }

   return map;
}


void convertFile(char * fileName, char * newFile) {
   FILE * pf = NULL; /* file */
   FILE * pt = NULL; /* target */
   char c;
   int val;

   pf = fopen(fileName, "r");

   if (!pf) {
      fprintf(stderr, "file not found : %s\n", fileName);
   } else {
      pt = fopen(newFile, "w");
      if (!pt) {
         fprintf(stderr, "erreur ouverture %s\n", newFile);
      } else {
        /* longueur */
        fscanf(pf, "%d", &val);
        fprintf(pt, "%d ", val);
        /* hauteur */
        fscanf(pf, "%d", &val);
        fprintf(pt, "%d ", val);
        /* nbEnnemies */
        fscanf(pf, "%d", &val);
        fprintf(pt, "%d ", val);

         do {
            c = fgetc(pf);

            switch (c) {
               case 'm' :
                  fprintf(pt, "%d ", FIC_CODE_m);
                  break;
               case 's' :
                  fprintf(pt, "%d ", FIC_CODE_s);
                  break;
               case 'T' :
                  fprintf(pt, "%d ", FIC_CODE_T);
                  break;
               case 'A' :
                  fprintf(pt, "%d ", FIC_CODE_A);
                  break;
               case 'G' :
                  fprintf(pt, "%d ", FIC_CODE_G);
                  break;
               case 'D' :
                  fprintf(pt, "%d ", FIC_CODE_D);
                  break;
               case 'H' :
                  fprintf(pt, "%d ", FIC_CODE_H);
                  break;
               case 'B' :
                  fprintf(pt, "%d ", FIC_CODE_B);
                  break;
               case 'g' :
                  fprintf(pt, "%d ", FIC_CODE_g);
                  break;
               case 'd' :
                  fprintf(pt, "%d ", FIC_CODE_d);
                  break;
               case 'h' :
                  fprintf(pt, "%d ", FIC_CODE_h);
                  break;
               case 'b' :
                  fprintf(pt, "%d ", FIC_CODE_b);
                  break;
               case 'Q' :
                  fprintf(pt, "%d ", FIC_CODE_Q);
                     fscanf(pf, "%d", &val);
                     fprintf(pt, "%d ", val);
                     fscanf(pf, "%d", &val);
                     fprintf(pt, "%d ", val);
                  break;
               case 'q' :
                  fprintf(pt, "%d ", FIC_CODE_q);
                  break;
               default:
                  break;
            }
         } while (!feof(pf));

         fclose(pt);
      }
      fclose(pf);
   }
}


block_t new_block(int id, int opt, block_opt_t * b) {
   block_t block;

   block.id = id;
   block.opt = opt;
   block.t = NULL;
   block.opt_data = b;

   return block;
}


block_opt_t * new_block_opt(int v1, int v2) {
   block_opt_t * block = (block_opt_t *) malloc(sizeof(*block));

   if (block) {
      block->v1 = v1;
      block->v2 = v2;
   }

   return block;
}

void freeMap(map_t map) {
   int i;

/* liberation des blocks d'option a faire */

   for (i = 0; i < map.w; ++i) {
      free(map.map[i]);
   }

   free(map.ennemies);

}
