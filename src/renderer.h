#pragma once
#include "shapes.h"
#include "../Printer/src/Canvas.h"


Canvas* getScoreCanvas();
Canvas *getCurrentCanvas();
void initRenderer();
void renderCube(Point p,int x, int y);
void renderCubeChar(Point p,int x, int y, char* car);
void render(Shape* currentShape, Shape* previewShape, Point  *fallenCubes, int fallenCount);
void renderScore(int score, Shape* savedShape);
void destroyCube(Point cube);// draw full blocks where preview was to f;ash it
void flashPreview(Shape *currentShape);
void end(int score);
void center();
void startScreen();
