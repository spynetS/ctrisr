#ifndef SAVER_H_
#define SAVER_H_

#include <stdio.h>
#include <stdlib.h>


int save_highscore(const char* path,const int score);
int get_highscore(const char* path, int *score);


#endif // SAVER_H_
