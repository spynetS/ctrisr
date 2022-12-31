#ifndef t
#include<stdio.h>
#endif
#include "shapes.c"
#include <time.h>
#include <errno.h>

#define WIDTH 10
#define HEIGHT 20

Point fallenCubes[WIDTH*HEIGHT]; // all places 
int fallenCount = 0;
int score = 1;


Shape *currentShape;
Shape *previewShape;

int msleep(long msec);

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
void renderScore(){
    printf("███████████████████████\n");
    printf("█         Score       █\n");

    if(score < 10){
        printf("█          %d          █\n",score);
    }
    else if(score >= 100)
    {
        printf("█         %d         █\n",score);
    }
    else if(score >= 10)
    {
        printf("█          %d         █\n",score);
    }
    printf("█                     █\n");
    printf("███████████████████████\n");
}

void setPreview(Shape shape){
    
    for(int i = 0; i < 4; i++){
        for(int r = 0; r < 20-currentShape->pos.y; r++){
            if(collides(*currentShape,newPoint(0, r), fallenCubes,fallenCount)){
                previewShape = newShape(shape.pos.x, currentShape->pos.y+r-1, shape.type);
                return;
            }
        }
    }

}

void renderWorld(Shape *currentShape){
    system("clear");
    renderScore();
    for(int y = 0; y < HEIGHT; y++){
        printf("█ ");
        for(int x = 0; x < WIDTH; x++){
            //render current shape
            int at = 0;
            for(int i = 0; i < 4; i++){
                Point cube = currentShape->cubes[i];
                Point pcube = previewShape->cubes[i];
                if( currentShape->pos.x+cube.x == x &&
                        currentShape->pos.y+cube.y == y){
                    renderPoint(cube);
                    at = 1;
                }
            }
            if(!at){
                for(int i = 0; i < 4; i++){
                    Point cube = currentShape->cubes[i];
                    if( previewShape->pos.x+cube.x == x &&
                            previewShape->pos.y+cube.y == y){
                        renderPointChar(cube,'=');
                        at = 1;
                    }
                }

            }
            for(int j = 0; j < fallenCount; j++){
                Point cube = fallenCubes[j];
                if( cube.x == x &&
                        cube.y == y){
                    renderPoint(cube);
                    at = 1;
                }
            }
            if(!at)
                printf(". ");
        }
        printf("█\n");
    }
    printf("███████████████████████\n");
}

int removeFullRow(){
    int count = 0;
    int rows = 0;
    for(int r = 20; r >= 0; r--){
        count = 0; 
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
                    renderWorld(currentShape);
                    fallenCubes[c].y = 100;
                    msleep(70);
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
    return rows;
}

int main(){
    
    currentShape = newShape(1,15,0);
    previewShape = newShape(1,16,0);
    int x = 8;

    while(1){
        score += removeFullRow()*10;

        setPreview(*currentShape);
        renderWorld(currentShape);
        char key = getchar();
        if(!collides(*currentShape, newPoint(0,1), &fallenCubes[0], fallenCount)){
            if(key == 'a' && !collides(*currentShape,newPoint(-1,0),&fallenCubes[0],fallenCount)){
                currentShape->pos.x--;
            }
            else if(key == 'd' && !collides(*currentShape,newPoint(1,0),&fallenCubes[0],fallenCount)){
                currentShape->pos.x++;
            }
            else if(key == 'w'){
                // if shape collides when rotate move it either right or left
                while(rotateCollide(*currentShape)){
                    if(currentShape->pos.x < 5){
                        currentShape->pos.x++;
                    }else{
                        currentShape->pos.x--;
                    }
                }
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
            int number = (rand() % (5 - 0 + 1)) + 0;
            currentShape = newShape(5,5,number);
            previewShape = newShape(5,5,number);
        }
        printf("\n");
        
        renderWorld(currentShape);
    }

    return 0;
}

