#ifndef RENDER_H_
#define RENDER_H_

#include "../lib/Canvas.h"
#include "shapes.h"



void setCube(Canvas *c, Point cube);

void setShape(Shape* currentShape, Canvas* c);
void renderWorld(Canvas* c, Shape* currentShape, Shape* previewShape ,Point** fallenCubes, int fallCount);

void renderSaved(Canvas* saved, Shape shape);
void renderNext(Canvas* c, int nexts[]);

#endif // RENDER_H_
