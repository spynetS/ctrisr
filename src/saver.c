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


int close_file(FILE* file){
   fclose(file);
   return 1;
}
FILE* open_file(const char* path){
   // use appropriate location if you are using MacOS or Linux
  FILE* file;
  file = fopen(path,"w");

  if(file == NULL)
  {
     printf("Error!");
     return NULL;
  }
  return file;
}

int save_highscore(FILE* file, const int score){
  return fprintf(file,"%d",score);
}
int create_highscore(FILE* file){
  return 0;
}
int get_highscore(FILE* file, int *score){
  int tmp;
  fscanf(file,"%d",&tmp);
  printf("%d\n",tmp);
  *score = tmp;
  return 1;
}
