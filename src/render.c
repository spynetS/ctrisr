#include "../lib/Canvas.h"
#include <stdio.h>
#include "shapes.h"
char* PIXEL = "⬜";

// RENDER FUNCTIONS
void setCube(Canvas *c, Point cube){
  setPixel(c, cube.x, cube.y, PIXEL, cube.color,BG_BLACK);
}

void setShape(Shape* currentShape, Canvas* c){

  for(int i = 0; i < 4; i ++){
    setPixel(c,currentShape->cubes[i].x+currentShape->pos.x,
            currentShape->cubes[i].y+currentShape->pos.y,
            PIXEL,currentShape->cubes[i].color,BG_BLACK);
  }
}


void renderSaved(Canvas* saved, Shape shape){

    shape.pos.y = 0;
    shape.pos.x = 0;
    setShape(&shape,saved);
    draw(saved);
}
void renderNext(Canvas* c, int nexts[]){

  clearPixels(c);
  setText(c,-1,0,"Next",WHITE,BG_BLACK);
  setShape(newShape(0,1,nexts[0]),c);
  setShape(newShape(0,4,nexts[1]),c);
  setShape(newShape(0,8,nexts[2]),c);

  draw(c);

}

void renderWorld(Canvas* c, Shape* currentShape,Shape* previewShape, Point** fallenCubes, int fallCount){


    //draw shape
    setShape(currentShape,c);
    setShape(previewShape,c);
    //setAllFallen
    for(int i = 0; i < fallCount; i++){
      setCube(c,*fallenCubes[i]);
    }

    draw(c);
    clearPixels(c);

}
