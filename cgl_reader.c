#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cgl_reader.h"
#include "freecg/cgl.h"

int main(int argc, char *argv[]){
   int num;
   FILE *fptr;

   printf("argument:%s\n", argv[1]);

   read_cgl(argv[1], NULL);

   // use appropriate location if you are using MacOS or Linux
   fptr = fopen("test.txt","w");

   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }

   printf("Enter num: ");
   scanf("%d",&num);

   fprintf(fptr,"%d",num);
   fclose(fptr);

   return 0;
}