#pragma once

typedef struct {
    int x;
    int y;
    char *color;
} Point;

typedef struct {
    Point pos; // global space
    Point cubes[4]; //cubes localspace
    int type;
} Shape;

Point newPoint(int x, int y);
Shape *newShape(int x, int y, int type);
int collides(Shape currentShape,Point nextStep ,Point *fallenCubes, int fallenCount);
void rotate(Shape *shape);
int rotateCollide(Shape shape,Point* fallenCubes,int count);
int rotateCollide(Shape shape,Point* fallenCubes,int count);
void renderPointChar(Point point, char* car);
