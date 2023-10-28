#include "../lib/Canvas.h"
#include "shapes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int HEIGHT = 15;
Canvas* c;
Shape* currentShape;
Point* fallenCubes[15*10];
int fallCount = 0;
char* PIXEL = "⬜";

// -- RENDER FUNCTIONS --
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

void render(){

    char buffer[10];
    sprintf(buffer,"%d",fallCount);

    setText(c,0,0,buffer,BLUE,BG_BLACK);

    //draw shape
    setShape(currentShape,c);
    //setAllFallen
    for(int i = 0; i < fallCount; i++){
      setCube(c,*fallenCubes[i]);
    }

    draw(c);
    clearPixels(c);

}
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
    strcpy(cube->color, "\033[36m");

    fallenCubes[fallCount] = cube;
    fallCount++;
    }
    freeShape(currentShape);
    currentShape = newShape(3,0,0);
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
      removeRow(i);
      dropAbove(i);
    }
  }
}


int main(){
  c = newCanvas(10,HEIGHT,"  ",BLACK,BG_BLACK);
  c->x = termWidth()/2-5;
  c->y = termHeight()/2-(HEIGHT/2);

  currentShape = newShape(0,0,0);
  currentShape->pos.x = 3;
  currentShape->pos.y = 3;


  int tick = 0;
  while(1){
    //check if there is a full row
    fullRow();

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
    }

    if(tick % 10 == 0){
      currentShape->pos.y++;
    }

    for(int i = 0; i < 4; i ++){
        if(currentShape->cubes[i].y+currentShape->pos.y >= HEIGHT-2 ||
           collides(*currentShape,newPoint(0,1),fallenCubes,fallCount)){
          newCurr();
        }
    }
    render();
    msleep(30);
    tick++;
  }

  return 0;
}
