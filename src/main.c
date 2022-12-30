#include<stdio.h>
#include "shapes.c"
#include <time.h>
#include <errno.h>

#define WIDTH 10
#define HEIGHT 20

Shape fallenShapes[WIDTH*HEIGHT/4]; // all places for in every shape
int fallenCount = 0;

int msleep(long msec);

void renderWorld(Shape *currentShape){
    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH; x++){
            //render current shape
            for(int i = 0; i < 4; i++){
                Point cube = currentShape->cubes[i];
                if( currentShape->pos.x+cube.x == x &&
                        currentShape->pos.y+cube.y == y){
                    printf("x");
                    x++;
                }
            }
            int at = 0;
            for(int i = 0; i < fallenCount; i++){
                Shape fallenShape = fallenShapes[i];
                for(int j = 0; j < 4; j++){
                    Point cube = fallenShape.cubes[j];
                    if( fallenShape.pos.x+cube.x == x &&
                            fallenShape.pos.y+cube.y == y){
                        printf("@");
                        at = 1;
                    }
                    
                }
            }
            if(!at)
                printf("-");
        }
        printf("\n");
    }
}


int main(){
    
    Shape *currentShape = newShape(5,10,0);
    int x = 8;

    while(1){
        if(!collides(*currentShape, newPoint(0,1), &fallenShapes[0], fallenCount)){
            currentShape->pos.y++;
        }else{
            fallenShapes[fallenCount] = *currentShape;
            fallenCount++;
            x--;
            currentShape = newShape(x,0,0);
        }
        
        printf("\n");
        renderWorld(currentShape);
        msleep(200);
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
