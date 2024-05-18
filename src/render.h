#ifndef RENDER_H_
#define RENDER_H_

#include "../lib/printer.h"
#include "shapes.h"



void setCube(Canvas *c, Point cube);
void setCubeWithChar(Canvas *c, Point cube,char* pixel);

void setShape(Shape* currentShape, Canvas* c);
void setShapeWithChar(Shape* currentShape, Canvas* c, char* pixel);
void renderWorld(Canvas* c, Shape* currentShape, Shape* previewShape ,Point** fallenCubes, int fallCount);

void renderSaved(Canvas* saved, Shape shape);
void renderNext(Canvas* c, int nexts[]);
void renderScore(Canvas* scoreCanvas,int score,int highscore,int rows);

#endif // RENDER_H_
