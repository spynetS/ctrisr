#ifndef a
#include <string.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "shapes.h"


Point newPoint(int x, int y){
    
    Point point;
    point.x = x;
    point.y = y;
    point.color = malloc(sizeof(char)*11);
    return point;
}

Shape *newShape(int x, int y, int type){
    
    Shape *shape = malloc(sizeof(Shape));
    shape->pos = newPoint(x,y);
    

    //memset(shape->color, 0, sizeof(shape->color));
    //I
    shape->type = type;
    if(type == 0 || type == 6){
        shape->cubes[0] = newPoint(-1,0);
        shape->cubes[1] = newPoint(0,0);
        shape->cubes[2] = newPoint(1,0);
        shape->cubes[3] = newPoint(2,0);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[0;36m");
        }
    }
    //L
    if(type == 1){
        shape->cubes[0] = newPoint(-1,0);
        shape->cubes[1] = newPoint(0,0);
        shape->cubes[2] = newPoint(1,0);
        shape->cubes[3] = newPoint(1,1);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[31;1m");
        }
    }
    
    //J
    if(type == 2){
        shape->cubes[0] = newPoint(1,-1);
        shape->cubes[1] = newPoint(-1,0);
        shape->cubes[2] = newPoint(0,0);
        shape->cubes[3] = newPoint(1,0);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[0;34m");
        }
    }
    //t
    if(type == 3){
        shape->cubes[0] = newPoint(0,0);
        shape->cubes[1] = newPoint(-1,0);
        shape->cubes[2] = newPoint(0,1);
        shape->cubes[3] = newPoint(1,0);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[0;35m");
        }
    }
    //o
    if(type == 4){
        shape->cubes[0] = newPoint(0,0);
        shape->cubes[1] = newPoint(1,1);
        shape->cubes[2] = newPoint(0,1);
        shape->cubes[3] = newPoint(1,0);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[0;33m");
        }
    }
    //s
    if(type == 5){
        shape->cubes[0] = newPoint(-1,0);
        shape->cubes[1] = newPoint(0,0);
        shape->cubes[2] = newPoint(0,1);
        shape->cubes[3] = newPoint(1,1);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[0;32m");
        }
    }
    //z
    if(type == 5){
        shape->cubes[0] = newPoint(-1,1);
        shape->cubes[1] = newPoint(0,1);
        shape->cubes[2] = newPoint(1,0);
        shape->cubes[3] = newPoint(0,0);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[0;31m");
        }
    }
    return shape;
}
/**
 *  nextStep = where to move the shape
 * */
int collides(Shape currentShape,Point nextStep ,Point *fallenCubes, int fallenCount){
    //update pos
    currentShape.pos.y += nextStep.y;
    currentShape.pos.x += nextStep.x;
    
    for(int sc = 0; sc < 4; sc++){ // for every cube in currentShape shape cube

        Point shapeCube = currentShape.cubes[sc]; 
        if(shapeCube.y + currentShape.pos.y == 20) return 1; // hit the bottom
        if(shapeCube.x + currentShape.pos.x <  1 ) return 1;
        if(shapeCube.x + currentShape.pos.x >  10) return 1;

        for(int fc = 0; fc < fallenCount; fc++){ // for every cube in fallenShape fallen cube
            Point fallenCube = fallenCubes[fc]; //that cube

            // if eny cube has the same pos it collides
            if(fallenCube.x == shapeCube.x+currentShape.pos.x &&
                    fallenCube.y == shapeCube.y+currentShape.pos.y){
                return 1;
            }
        }
    }
    //else no collision
    return 0;
}

void rotate(Shape *shape){
    for(int i = 0; i < 4; i++){
        Point *cube = &shape->cubes[i];

        int x_temp = -cube->y;
        cube->y =  cube->x;
        cube->x =  x_temp;
    }
}

int rotateCollide(Shape shape,Point* fallenCubes,int count){
    Shape *shape_p = &shape; 
    rotate(shape_p);
    return collides(*shape_p, newPoint(0,0),fallenCubes, count);
}

void renderPoint(Point point){
    /* printf("%s▄ ", point.color); */
    /* printf("%s■ ", point.color); */
    printf("%s██", point.color);
    printf("\033[0m");
}
void renderPointChar(Point point, char* car){
    printf("%s", point.color);
    printf("%s",car);
    /* printf("%s■ ", point.color); */
    printf("\033[0m");
}
