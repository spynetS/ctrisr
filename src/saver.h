#ifndef SAVER_H_
#define SAVER_H_

#include <stdio.h>
#include <stdlib.h>


int save_highscore(FILE* file,const int score);
int create_highscore(FILE* file);
int get_highscore(FILE* file, int *score);
FILE* open_file(const char* path);
int close_file(FILE* file);


#endif // SAVER_H_
