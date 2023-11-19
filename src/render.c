#include "../lib/Canvas.h"
#include <stdio.h>
#include "shapes.h"
//char* PIXEL = "⬜";
char* PIXEL = "██";

// RENDER FUNCTIONS
void setCubeWithChar(Canvas *c, Point cube,char* pixel){
  setPixel(c, cube.x, cube.y, pixel, cube.color,BG_BLACK);
}
void setCube(Canvas *c, Point cube){
  setCubeWithChar(c,cube,PIXEL);
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

void renderScore(Canvas* scoreCanvas,int score,int highscore,int rows){
  clearPixels(scoreCanvas);
  setCenterText(scoreCanvas,scoreCanvas->width/2-1,0,"Highscore",WHITE,BG_BLACK);
  char sscore[10];
  sprintf(sscore,"%d",highscore);
  setCenterText(scoreCanvas,scoreCanvas->width/2-1,1,sscore,WHITE,BG_BLACK);

  setCenterText(scoreCanvas,scoreCanvas->width/2-1,2,"Score",WHITE,BG_BLACK);
  sprintf(sscore,"%d",score);
  setCenterText(scoreCanvas,scoreCanvas->width/2-1,3,sscore,WHITE,BG_BLACK);

  setCenterText(scoreCanvas,scoreCanvas->width/2-1,4,"Lines",WHITE,BG_BLACK);
  //char srows[10];
  sprintf(sscore,"%d",rows);
  setCenterText(scoreCanvas,scoreCanvas->width/2-1,5,sscore,WHITE,BG_BLACK);

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
    if (previewShape != NULL)
      setShapeWithChar(previewShape,c,"[]");
    if (currentShape != NULL)
      setShape(currentShape,c);
    //setAllFallen
    for(int i = 0; i < fallCount; i++){
      setCube(c,*fallenCubes[i]);
    }

    setBorder(c,1);
    draw(c);
    clearPixels(c);
}
