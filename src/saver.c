/*
 2023 Alfred Roos
 example of use
 int main(){
   FILE *file;
   int score = 0;

   file = open_file(path);
   printf("%d\n",score);

   save_highscore(file,10);
   get_highscore(file,&score);

   printf("%d\n",score);
   close_file(file);

   return 0;
 }
*/

#include "saver.h"


int save_highscore(const char* path,const int score){
  FILE* file;
  file = fopen(path,"w");

  if(file == NULL)
  {
     printf("Error!");
     return 0;
  }
  fprintf(file,"%d",score);
  fclose(file);
  return 1;
}
int get_highscore(const char* path, int *score){
  FILE* file;
  file = fopen(path,"r");

  if(file == NULL)
  {
     printf("Error!");
     return 0;
  }
  fscanf(file,"%d",score);
  fclose(file);
  return 1;
}
