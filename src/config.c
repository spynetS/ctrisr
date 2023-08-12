#include "config.h"
#include <string.h>
#include <stdlib.h>
#include "../flagcer/flagser.h"

int music = 0;

// char* cubeStr         = "██";//⏹
char* cubeStr         = "⏹ ";//⏹
// char* cubeStr         = "🟦";//⏹
char* wallStr         = "▣ ";
char* destroyCubeStr1 = "▓█";
char* destroyCubeStr2 = "▒▒";
char* destroyCubeStr3 = "░░";
char* flashCubeStr    = "██";

void args(int argc, char **argv){
    addFlag("-m", "--music", "when set to 1 music is on and 0 it is off", setMusic);
    addFlag("-b", "--block", "set the unicode for block", setBlock);
    addHelp();
    parse(argc, argv); 
}

void setMusic(int argc, char **argv){
    if(strcmp(argv[1], "0") != 0)
        music = 1;
    else 
        music = 0;
}

void setBlock(int argc, char **argv){
}
