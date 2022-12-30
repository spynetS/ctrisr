#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point pos; // global space
    Point cubes[4]; //cubes localspace
} Shape;

Point newPoint(int x, int y){
    
    Point point;
    point.x = x;
    point.y = y;
    return point;
}

Shape *newShape(int x, int y, int type){
    
    Shape *shape = malloc(sizeof(Shape));
    shape->pos = newPoint(x,y);
    
    /* Point cubes[4] = {newPoint(-1,0),newPoint(0,0),newPoint(1,0),newPoint(2,0)}; */
    /* Point cubes[4] = {newPoint(-1,1),newPoint(0,1),newPoint(0,0),newPoint(1,1)}; */
    /* Point cubes[4] = {newPoint(-1,0),newPoint(0,0),newPoint(1,0),newPoint(1,-1)}; */
    Point cubes[4] = {newPoint(-1,0),newPoint(0,0),newPoint(1,0),newPoint(2,0)};
    for(int i = 0; i < 4;i++){
        shape->cubes[i] = cubes[i];
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
        if(shapeCube.x + currentShape.pos.x < 0) return 1;
        if(shapeCube.x + currentShape.pos.x >=10) return 1;

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


