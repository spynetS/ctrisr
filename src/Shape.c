#ifndef a
#include <string.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "shapes.h"
#include "../lib/printer.h"


void freeShape(Shape* shape){

  free(shape->pos.color);
  for(int i = 0; i < 4; i++){
    free(shape->cubes[i].color);
  }
  free(shape);

}
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
    if(type == 0 ){
        shape->cubes[0] = newPoint(-1,0);
        shape->cubes[1] = newPoint(0,0);
        shape->cubes[2] = newPoint(1,0);
        shape->cubes[3] = newPoint(2,0);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[36m");
        }
    }
    //L
    else if(type == 1){
        shape->cubes[0] = newPoint(-1,0);
        shape->cubes[1] = newPoint(0,0);
        shape->cubes[2] = newPoint(1,0);
        shape->cubes[3] = newPoint(1,1);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[31m");
        }
    }

    //J
    else if(type == 2){
        shape->cubes[0] = newPoint(1,-1);
        shape->cubes[1] = newPoint(-1,0);
        shape->cubes[2] = newPoint(0,0);
        shape->cubes[3] = newPoint(1,0);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[34m");
        }
    }
    //t
    else if(type == 3){
        shape->cubes[0] = newPoint(0,0);
        shape->cubes[1] = newPoint(-1,0);
        shape->cubes[2] = newPoint(0,1);
        shape->cubes[3] = newPoint(1,0);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[35m");
        }
    }
    //o
    else if(type == 4){
        shape->cubes[0] = newPoint(0,0);
        shape->cubes[1] = newPoint(1,1);
        shape->cubes[2] = newPoint(0,1);
        shape->cubes[3] = newPoint(1,0);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[33m");
        }
    }
    //s
    else if(type == 5){
        shape->cubes[0] = newPoint(-1,0);
        shape->cubes[1] = newPoint(0,0);
        shape->cubes[2] = newPoint(0,1);
        shape->cubes[3] = newPoint(1,1);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[32m");
        }
    }
    //z
    else if(type == 6){
        shape->cubes[0] = newPoint(-1,1);
        shape->cubes[1] = newPoint(0,1);
        shape->cubes[2] = newPoint(1,0);
        shape->cubes[3] = newPoint(0,0);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, RED);
        }
    }
    else {
        shape->cubes[0] = newPoint(-100,1);
        shape->cubes[1] = newPoint(100,1);
        shape->cubes[2] = newPoint(100,0);
        shape->cubes[3] = newPoint(100,0);
        for(int i = 0; i < 4; i++){
            strcpy(shape->cubes[i].color, "\033[31m");
        }
    }
    return shape;
}
/**
 *  nextStep = where to move the shape
 * */
int collides(Shape currentShape,Point nextStep ,Point **fallenCubes, int fallenCount){
    //update pos
    currentShape.pos.y += nextStep.y;
    currentShape.pos.x += nextStep.x;

    for(int sc = 0; sc < 4; sc++){ // for every cube in currentShape shape cube

        Point shapeCube = currentShape.cubes[sc];
        if(shapeCube.y + currentShape.pos.y == 22) return 1; // hit the bottom
        if(shapeCube.x + currentShape.pos.x <  0) return 1;
        if(shapeCube.x + currentShape.pos.x >  9) return 1;

        for(int fc = 0; fc < fallenCount; fc++){ // for every cube in fallenShape fallen cube
            Point *fallenCube = fallenCubes[fc]; //that cube

            // if eny cube has the same pos it collides
            if(fallenCube->x == shapeCube.x+currentShape.pos.x &&
                    fallenCube->y == shapeCube.y+currentShape.pos.y){
                return 1;
            }
        }
    }
    //else no collision
    return 0;
}

void rotate(Shape *shape){
    // the o cant be rotated
    if(shape->type != 4){
        for(int i = 0; i < 4; i++){
            Point *cube = &shape->cubes[i];

            int x_temp = -cube->y;
            cube->y =  cube->x;
            cube->x =  x_temp;
        }
    }
}

int rotateCollide(Shape shape,Point** fallenCubes,int count){
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
