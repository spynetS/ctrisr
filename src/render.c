#include "../lib/Canvas.h"
#include <stdio.h>
#include "shapes.h"
//char* PIXEL = "⬜";
char* PIXEL = "██";

// RENDER FUNCTIONS
void setCube(Canvas *c, Point cube){
  setPixel(c, cube.x, cube.y, PIXEL, cube.color,BG_BLACK);
}
void setShapeWithChar(Shape* currentShape, Canvas* c, char* pixel){

  for(int i = 0; i < 4; i ++){
    setPixel(c,currentShape->cubes[i].x+currentShape->pos.x,
            currentShape->cubes[i].y+currentShape->pos.y,
            pixel,currentShape->cubes[i].color,BG_BLACK);
  }
}

void setShape(Shape* currentShape, Canvas* c){

  for(int i = 0; i < 4; i ++){
    setPixel(c,currentShape->cubes[i].x+currentShape->pos.x,
            currentShape->cubes[i].y+currentShape->pos.y,
            PIXEL,currentShape->cubes[i].color,BG_BLACK);
  }
}

void renderScore(Canvas* scoreCanvas,int score){
  clearPixels(scoreCanvas);
  setText(scoreCanvas,0,0,"Score",WHITE,BG_BLACK);
  char sscore[10];
  sprintf(sscore,"%d",score);
  setText(scoreCanvas,0,1,sscore,WHITE,BG_BLACK);

  draw(scoreCanvas);
  setBorder(scoreCanvas,1);
}

void renderSaved(Canvas* saved, Shape shape){

    shape.pos.y = 2;
    shape.pos.x = 1;
    setShape(&shape,saved);

    setText(saved, 0,0,"Hold",WHITE,BG_BLACK);

    setBorder(saved,1);
    draw(saved);
}
void renderNext(Canvas* c, int nexts[]){

  clearPixels(c);
  setText(c,0,0,"Next",WHITE,BG_BLACK);
  setShape(newShape(1,2,nexts[0]),c);
  setShape(newShape(1,6,nexts[1]),c);
  setShape(newShape(1,10,nexts[2]),c);

  setBorder(c,1);
  draw(c);

}

void renderWorld(Canvas* c, Shape* currentShape,Shape* previewShape, Point** fallenCubes, int fallCount){


    setText(c,2,0,"CTRISR",WHITE,BG_BLACK);
    //draw shape
    setShape(currentShape,c);
    setShapeWithChar(previewShape,c,"[]");
    //setAllFallen
    for(int i = 0; i < fallCount; i++){
      setCube(c,*fallenCubes[i]);
    }

    setBorder(c,1);
    draw(c);
    clearPixels(c);

}
