#include "shapes.h"
#include "msc.c"
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include <pthread.h>
#include "sound.h"
#include "config.h"
#include "../flagcer/flagser.h"

#define WIDTH 11
#define HEIGHT 22


int paused = 0;
Point fallenCubes[1000]; // all places 
int fallenCount = 0; //keep count of how many has fallen
int score       = 0; 
int fallDelay   = 20; // how long to wait to fall
int fallDeleyDecreser = 500; // when zero decrese fallDelay
struct winsize w;

Shape *currentShape;
Shape *previewShape;
Shape *savedShape;

void renderWorld(Shape *currentShape){


    render(currentShape,previewShape, fallenCubes, fallenCount); 
    renderScore(score,savedShape);
}

void setPreview(Shape shape){
    // loop though all cubes 
    for(int i = 0; i < 4; i++){
        //check if it collides att bottom and upwards
        for(int r = 0; r < HEIGHT-currentShape->pos.y; r++){
            // if collides set the previewShape position
            if(collides(*currentShape,newPoint(0, r+1), fallenCubes,fallenCount)){
                previewShape = newShape(shape.pos.x, currentShape->pos.y+r, shape.type);
                return;
            }
        }
    }
}


int removeFullRow(){
    int count = 0; // amount of cubes on a row
    int rows = 0; // rows that are full
    int removed = 1;
    //begin from bottom (want to remove bottom up)
    for(int r = HEIGHT; r >= 0; r--){
        count = 0;
        //count amount on row 
        for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
            if(fallenCubes[c].y == r){
                count++;
            }
        }
        if(count == 10){
            //remove chars
            rows++;
            for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
                if(fallenCubes[c].y == r){
                    // should free this value instead of move
                    destroyCube(fallenCubes[c]);

                    BeepThread(300+(removed * 10), 100);
                    if(removed == 39){
                        BeepThread(700, 800);
                    }

                    fallenCubes[c].y = 100;

                    renderWorld(currentShape);
                    printer_msleep(30);
                    removed ++;
                }
            }
            //move above down
            for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
                if(fallenCubes[c].y < r){
                    fallenCubes[c].y++; 
                }
                //renderWorld(currentShape);
            }
            r++;
        }
    }
    setPreview(*currentShape);
    return rows;
}

void updateScore(int rows){
    if(rows == 1) score += 40;
    if(rows == 2) score += 100;
    if(rows == 3) score += 300;
    if(rows == 4) score += 1200;
}

void setNewShape(){
    // get random number (shape)
    srand(time(0));
    int number = (rand() % (5 - 0 + 1)) + 0;
    currentShape = newShape(4,-1,number);
    previewShape = newShape(4,-1,number);
    setPreview(*currentShape); 
}


void *myThreadFun(void *vargp)
{
    if(music == 1){
        while(1){
            playMusic();
        }
    }
    
    return NULL;
}


int main(int argc, char **argv){
    
    
    args(argc, argv);

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, myThreadFun, NULL);

    /*
     * create a nw thread and start playing the music there
     */  
    // retrive terminal width and height
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); 
   
    startScreen();

    //create current shape
    currentShape = newShape(5,1,2);
    //create a placed preview
    previewShape = newShape(1,16,0);

    int renderTime = 0; 
    while(1){
        printer_msleep(50);
        if(fallDeleyDecreser == 0){
            fallDelay --;
            fallDeleyDecreser = 500;
        }
        fallDeleyDecreser--;
        //amount of rows deleted addes score
        updateScore(removeFullRow());


        //key pressed?
        if(printer_kbhit()){
            //get key
            char key = getchar();
            if(key == 'q'){
                end(score);
            }
            if(key == 'e'){
                //save shape
                if(savedShape == NULL){
                    savedShape = newShape(0,0,currentShape->type);
                    setNewShape();
                }
                //retrive
                else{
                    Shape *temp = currentShape;
                    currentShape = newShape(currentShape->pos.x, currentShape->pos.y,savedShape->type);
                    // currentShape->pos.y = temp->pos.y;
                    // currentShape->pos.x = temp->pos.x;
                    savedShape = newShape(0,0,temp->type);
                }
            }
            if(key == 'p'){
                paused = paused==1?0:1;
            }
            if(key == 'a' && !collides(*currentShape,newPoint(-1,0),&fallenCubes[0],fallenCount) && !paused){
                currentShape->pos.x--;
            }
            if(key == 's' && !collides(*currentShape,newPoint(0,1),&fallenCubes[0],fallenCount)&& !paused){
                //fallDelay = 0;
                currentShape->pos.y ++;
                score++;
            }
            if(key == ' ' && !collides(*currentShape,newPoint(0,1),&fallenCubes[0],fallenCount)&& !paused){
                //fallDelay = 0;
                currentShape->pos.y = previewShape->pos.y;
                
                flashPreview(currentShape);

                score++;
            }
            if(key == 'd' && !collides(*currentShape,newPoint(1,0),&fallenCubes[0],fallenCount)&& !paused){
                currentShape->pos.x++;
            }
            else if(key == 'w'&& !paused){
                // if shape collides when rotate move it either right or left
                for(int i = 0; i < 4; i++){ // 4 is max length
                    if(rotateCollide(*currentShape, &fallenCubes[0], fallenCount)){
                        if(currentShape->pos.x < 5 && !collides(*currentShape, newPoint(1,0),&fallenCubes[0],fallenCount) ){
                            currentShape->pos.x++;
                        }else if(!collides(*currentShape, newPoint(-1,0),&fallenCubes[0],fallenCount)){
                            currentShape->pos.x--;
                        }
                    }else{
                        rotate(currentShape);
                        break;
                    }
                }
                // then rotate
            }

            //calculate where places preview should be
            setPreview(*currentShape); 
            renderWorld(currentShape);
        }
        if(!paused){

            // move shape down
            if(!collides(*currentShape, newPoint(0,1), &fallenCubes[0], fallenCount) ){
                if(renderTime > fallDelay){
                    currentShape->pos.y++;
                    renderWorld(currentShape);
                    renderTime =0;
                }
            }else{
                //end score
                if(currentShape->pos.y == -1){
                    end(score);
                }
                //add cubes to fallen array and add shape pos to them
                for(int i = 0; i < 4; i++){
                    Point newCube = currentShape->cubes[i];
                    newCube.x += currentShape->pos.x;
                    newCube.y += currentShape->pos.y;
                    fallenCubes[fallenCount] = newCube;
                    fallenCount++;
                }
                setNewShape();
                score++;
            }
            renderTime++;
        }
    }

    end(score);
}

