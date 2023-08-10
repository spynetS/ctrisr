#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flag{
    char* name;
    char* longname;
    char* description;
    void (*oncall)(int argc, char** args);
} Flag;

Flag flags[20];
int flaglength = 0;

void addFlag(char* name, char* longname, char* description, void (*oncall)(int argc, char** args)){
    Flag flag;
    flag.name = name;
    flag.longname = longname;
    flag.description = description;
    flag.oncall = oncall;

    flags[flaglength] = flag;
    flaglength++;
}

void printHelp(int argc, char** args){
    int nl = 0;
    int lnl = 0;
    //calculate longset names 
    for(int i = 0; i < flaglength; i++){
        if(strlen(flags[i].name)>nl) nl = strlen(flags[i].name);
        if(strlen(flags[i].longname)>lnl) lnl = strlen(flags[i].longname);
    }
    //adde some padding
    nl+=2;
    lnl+=5;
    //print the flags
    for(int i = 0; i < flaglength; i++){
        //name
        printf("%s", flags[i].name);
        //right amount of spaces 
        for(int j = 0; j < nl-strlen(flags[i].name); j++){
            printf(" ");
        }
        //print longname
        printf("%s", flags[i].longname);
        //right amount of spaces
        for(int j = 0; j < lnl-strlen(flags[i].longname); j++){
            printf(" ");
        }
        //description
        printf("%s\n", flags[i].description);
    }
}

void print(int argc, char** args){
    for(int i = 1; i < argc; i++){
        printf("%s\n", args[i]);
    }
}

#define LISTEN 1
//returns amount of flags found
int parse(int argc, char** argv){
    // after first is 1
    int state = 0;
    // the args we pass to the lambda functions
    int argsindex = 0;
    char** args = malloc(sizeof(char)*300);
    //last arg is to when we have checked all args we call the last with its args 
    int lastarg = 1;
        
    int flagsFound = 0;

    for(int i = 0; i < argc; i++){
        for(int j = 0; j < flaglength; j++){
            // we find a flag
            if(strcmp(argv[i],flags[j].name) == 0 ||
               strcmp(argv[i],flags[j].longname) == 0){
                //if we have listen we call the args and pass its args to it
                if(state == LISTEN)
                    flags[lastarg].oncall(argsindex, args);

                lastarg = j;

                flagsFound++;
                //reset args
                argsindex = 0;
                //set so we can begin to listen to args for it
                state = LISTEN;
                //move to next arg
                continue;
            }
        }
        if(state == LISTEN){
            //malloc size of our arg
            args[argsindex] = malloc(sizeof(char)*strlen(argv[i])+1);
            //clear the memory
            memset(args[argsindex], 0, sizeof(char)+1);
            //copy the value
            strcpy(args[argsindex], argv[i]);
            
            argsindex++;
        }
    }
    //run last arg
    if(flagsFound > 0)
        flags[lastarg].oncall(argsindex, args);
    //maybe free every string inside args?
    return flagsFound;
}

void addHelp(){
    addFlag("-h","--help", "outputs this message", printHelp);
}
