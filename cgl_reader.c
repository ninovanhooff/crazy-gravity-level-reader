#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cgl_reader.h"
#include "freecg/cgl.h"

#define for_each_item(item, list) \
    for(T * item = list->head; item != NULL; item = item->next)

#define foreach(item, array) \
    for(int keep = 1, \
            count = 0,\
            size = sizeof (array) / sizeof *(array); \
        keep && count != size; \
        keep = !keep, count++) \
      for(item = (array) + count; keep; keep = !keep)

int main(int argc, char *argv[]){
   printf("file path:%s\n", argv[1]);
   
   struct cgl *cgl = read_cgl(argv[1], NULL);
   printf("size: %zu, %zu\n", cgl->width, cgl->height);
   //printf("airports: %zu, %zu\n", cgl->airports->type, cgl->height);
   struct airport *aps = cgl->airports;
   printf("num aps: %zu\n", cgl->nairports);
   //struct airport ap;

   // for (size_t j = 0; j < cgl->nairports; ++j) {
   //    printf("num_cargo: %d\n", aps->num_cargo);
   //    aps++;
   // }
   
   for (struct airport *start = aps; aps < start + cgl->nairports; ++aps) {
      printf("num_cargo: %zu\n", aps->num_cargo);
   }

   return 0;
}