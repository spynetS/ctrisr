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
            PIXEL,BLUE,BG_BLACK);
  }
}

void renderWorld(Canvas* c, Shape* currentShape,Shape* previewShape, Point** fallenCubes, int fallCount){

    char buffer[10];
    sprintf(buffer,"%d",fallCount);

    setText(c,0,0,buffer,BLUE,BG_BLACK);

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
