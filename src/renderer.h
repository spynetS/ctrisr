#pragma once
#include "shapes.h"

void initRenderer();
void renderPoint(Point p);
void render(Shape* currentShape, Shape* previewShape, Point  *fallenCubes, int fallenCount);
