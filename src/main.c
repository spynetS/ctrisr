#include<stdio.h>
#include "shapes.c"
#include <time.h>
#include <errno.h>

#define WIDTH 10
#define HEIGHT 20

Point fallenCubes[WIDTH*HEIGHT]; // all places 
int fallenCount = 0;

Shape *currentShape;

int msleep(long msec);

void renderWorld(Shape *currentShape){
    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH; x++){
            //render current shape
            int at = 0;
            for(int i = 0; i < 4; i++){
                Point cube = currentShape->cubes[i];
                if( currentShape->pos.x+cube.x == x &&
                        currentShape->pos.y+cube.y == y){
                    if(cube.x == 0 && cube.y == 0){
                        printf("o");
                    }else{
                        printf("x");
                    }
                    at = 1;
                }
            }
            for(int j = 0; j < fallenCount; j++){
                Point cube = fallenCubes[j];
                if( cube.x == x &&
                        cube.y == y){
                    printf("@");
                    at = 1;
                }
                
            }
            if(!at)
                printf("-");
        }

        printf("\n");
    }
}
//check all rows if they are full and return the amount of rows that are full
// start at buttom, can stop when a row is empty
/* int fullRows(){ */
/*     int rows = 0; */
/*     int count = 0; // amount of cubes at current row */
/*     for(int r = 20; r >= 0; r--){ */
/*         count = 0; */
/*         for(int i = 0; i < fallenCount; i++){ */
/*             Shape fallen = fallenShapes[i]; */
/*             for(int c = 0; c < 4; c++){ */
/*                 Point cube = fallen.cubes[i]; */
/*                 if(cube.y == r) count++; */
/*             } */
/*         } */
/*         printf("count %d",count); */
/*         if(count == 10) rows++; */
/*     } */
/*     return rows; */
/* } */



void removeFullRow(){
    int count = 0;
    for(int r = 20; r >= 0; r--){
        count = 0; 
        for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
            if(fallenCubes[c].y == r){
                count++;
            }
        }
        if(count == 10){
            //remove chars
            for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
                if(fallenCubes[c].y == r){
                    renderWorld(currentShape);
                    fallenCubes[c].y = 100;
                    msleep(100);
                }
            }
            //move above down
            for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
                if(fallenCubes[c].y < r){
                    fallenCubes[c].y++; 
                }
                renderWorld(currentShape);
            }
            r++;
        }
    }
}

int main(){
    
    currentShape = newShape(1,15,0);
    int x = 8;

    while(1){
        removeFullRow();
        char key = getchar();
        if(!collides(*currentShape, newPoint(0,1), &fallenCubes[0], fallenCount)){
            if(key == 'a' && !collides(*currentShape,newPoint(-1,0),&fallenCubes[0],fallenCount)){
                currentShape->pos.x--;
            }
            else if(key == 'd' && !collides(*currentShape,newPoint(1,0),&fallenCubes[0],fallenCount)){
                currentShape->pos.x++;
            }
            else if(key == 'w'){
                rotate(currentShape);
            }
            else{
                currentShape->pos.y++;
            }
        }else{
            //add cubes to fallen array and add shape pos to them
            for(int i = 0; i < 4; i++){
                Point newCube = currentShape->cubes[i];
                newCube.x += currentShape->pos.x;
                newCube.y += currentShape->pos.y;
                fallenCubes[fallenCount] = newCube;
                fallenCount++;
            }
            currentShape = newShape(5,5,0);
        }
        printf("\n");
        
        renderWorld(currentShape);
    }

    return 0;
}


int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
