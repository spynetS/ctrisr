#include "../lib/Canvas.h"
#include "shapes.h"
#include "render.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int HEIGHT = 22;
Canvas* c;
Canvas* savedCanvas;
Canvas* nextCanvas;
Canvas* scoreCanvas;

Shape* currentShape;
Shape* previewShape;
Shape* savedShape;
int next[3]; // list containg the next shapes

Point* fallenCubes[15*10];
int fallCount = 0;

// -- LOGIC FUNCTIONS --
//adds all cubes from the currentShape to the array
//and sets a new currentShape
void newCurr(){
    for(int i = 0; i < 4; i++){
      int x = currentShape->cubes[i].x+currentShape->pos.x;
      int y = currentShape->cubes[i].y+currentShape->pos.y;
      Point *cube = malloc(sizeof(Point));
      cube->x=x;
      cube->y=y;
      cube->color = malloc(sizeof(char)*11);
      strcpy(cube->color, currentShape->cubes[i].color);

      fallenCubes[fallCount] = cube;
      fallCount++;
    }
    freeShape(currentShape);

    //set it to the first in the list
    currentShape = newShape(4,-1,next[0]);
    //update the next list
    srand(time(0));
    int number = (rand() % (5 - 0 + 1)) + 0;
    next[0] = next[1];
    next[1] = next[2];
    next[2] = number;
}
//returns 1 if there is a cube in the fallenCubes array with the position
int isThereAFallenCubeHere(int x, int y){
  for(int i = 0; i < fallCount; i ++){
    if(fallenCubes[i]->x == x && fallenCubes[i]->y==y){
      return 1;
    }
  }
  return 0;
}

//removes cubes on the row y coordinate
void removeRow(int row){
  //create a temp array and add all cubes not in the row
  Point* fallenCubesNew[10*15];
  int count = 0;
  for(int i = 0; i < fallCount; i++){
    if(fallenCubes[i]->y != row){
      fallenCubesNew[count] = fallenCubes[i];
      count++;
    }
  }
  // set all right cubes to the main array
  for(int i = 0; i < count; i ++){
    fallenCubes[i] = fallenCubesNew[i];
  }
  fallCount = count;
}

//makes the cubes above the row goes doen 1 unit
void dropAbove(int row){
  for(int i = 0; i < fallCount; i ++){
    if(fallenCubes[i]->y < row){
      fallenCubes[i]->y++;
    }
  }

}

//checks if a row is full if so remove it and make the above go down
void fullRow(){
  for(int i = HEIGHT; i >= 0; i --){
    int count = 0;
    for(int x = 0; x < 10; x++){
      if(isThereAFallenCubeHere(x,i)) count++;
    }
    if(count == 9){
      for(int j = 0; j < fallCount; j ++){
        if(fallenCubes[j]->y == i){
          fallenCubes[j]->y=100;

          clearPixels(c);
          renderWorld(c,currentShape,previewShape,fallenCubes,fallCount);
          msleep(40);
        }
      }
      removeRow(100);
      dropAbove(i);
    }
  }
}

int amountToFall(Shape shape){
  for(int i = shape.pos.y; i < HEIGHT;i ++){
    for(int j = 0; j < 4; j ++){
        if(shape.cubes[j].y+shape.pos.y+i >= HEIGHT-2 ||
           collides(shape,newPoint(0,i),fallenCubes,fallCount)){
          return i-1;
        }
    }
  }
  return shape.pos.y;
}

void movement(int tick){

    char keyInput = getKeyPressed();
    if(keyInput == 'd'){
      if(!collides(*currentShape,newPoint(1,0),fallenCubes,fallCount))
        currentShape->pos.x++;
    }
    if(keyInput == 'a'){
      if(!collides(*currentShape,newPoint(-1,0),fallenCubes,fallCount))
        currentShape->pos.x--;
    }
    if(keyInput == 'w'){
      if(!rotateCollide(*currentShape, fallenCubes,fallCount))
        rotate(currentShape);
        rotate(previewShape);
    }
    if(keyInput == 'e'){

      if(savedShape == NULL){
        savedShape = currentShape;
        currentShape = newShape(4,1,1);
      }
      else{
        Shape* temp = savedShape;
        temp->pos.x = currentShape->pos.x;
        temp->pos.y = currentShape->pos.y;
        savedShape = currentShape;
        currentShape = temp;
      }

    }
    if(keyInput == ' '){
      currentShape->pos.y = previewShape->pos.y;
    }
}

void setPreviewShape(Shape shape){
    // loop though all cubes
    for(int i = 0; i < 4; i++){
        //check if it collides att bottom and upwards
        for(int r = 0; r < HEIGHT-currentShape->pos.y; r++){
            // if collides set the previewShape position
            if( collides(*currentShape,newPoint(0, r+1), fallenCubes,fallCount)){

                previewShape->pos.x = currentShape->pos.x;
                previewShape->pos.y = currentShape->pos.y+r;

                for(int j = 0; j < 4; j ++){
                  previewShape->cubes[j] = currentShape->cubes[j];
                }

                return;
            }
        }
    }
}

int main(){
  next[0] = 0;
  next[1] = 1;
  next[2] = 2;

  c = newCanvas(10,HEIGHT,"  ",WHITE,BG_BLACK);
  c->x = termWidth()/2-5;
  c->y = termHeight()/2-(HEIGHT/2);
  setBorder(c,1);

  savedCanvas = newCanvas(4,4,"  ",WHITE,BG_BLACK);
  savedCanvas->x = termWidth()/2-14;
  savedCanvas->y = termHeight()/2 - (HEIGHT/2);

  nextCanvas = newCanvas(4,4*3, "  ", WHITE,BG_BLACK);
  nextCanvas->x = termWidth()/2+17;
  nextCanvas->y = termHeight()/2 - (HEIGHT/2);


  previewShape = newShape(0,0,0);
  currentShape = newShape(0,0,0);
  currentShape->pos.x = 3;
  currentShape->pos.y = 3;

  int tick = 0;
  while(1){
    //check if there is a full row
    fullRow();


    if(tick % 10 == 0){
      currentShape->pos.y++;
    }
    movement(tick);

    setPreviewShape(*currentShape);

    //check if the currentShape has fallen down
    for(int i = 0; i < 4; i ++){
        if(currentShape->cubes[i].y+currentShape->pos.y >= HEIGHT-2 ||
           collides(*currentShape,newPoint(0,1),fallenCubes,fallCount)){
          newCurr();
        }
    }
    renderWorld(c,currentShape,previewShape,fallenCubes,fallCount);

    renderNext(nextCanvas,next);

    clearPixels(savedCanvas);
    if(savedShape != NULL)
      renderSaved(savedCanvas, *savedShape);



    msleep(50);
    tick++;
  }

  return 0;
}
