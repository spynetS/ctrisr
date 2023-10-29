#ifndef SHAPE_H_
#define SHAPE_H_

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
void freeShape(Shape* shape);
int collides(Shape currentShape,Point nextStep ,Point **fallenCubes, int fallenCount);
void rotate(Shape *shape);
int rotateCollide(Shape shape,Point** fallenCubes,int count);
//int rotateCollide(Shape shape,Point* fallenCubes,int count);
void renderPoint(Point point);
void renderPointChar(Point point, char* car);

#endif // SHAPE_H_
