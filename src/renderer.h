#pragma once
#include "shapes.h"
#include "../Printer/src/Canvas.h"


Canvas *getCurrentCanvas();
void initRenderer();
void renderCube(Point p,int x, int y);
void renderCubeChar(Point p,int x, int y, char* car);
void render(Shape* currentShape, Shape* previewShape, Point  *fallenCubes, int fallenCount);
