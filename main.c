#include "./src/saver.h"


int main(){

  int score;
  get_highscore("/home/spy/dev/c/ctrisr/highscore.txt",&score);
  printf("%d\n",score);
  //close_file(file);

  return 0;
}
