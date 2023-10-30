#include "../lib/Canvas.h"
#include "shapes.h"
#include "render.h"
#include "sound.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int HEIGHT = 22;
Canvas* c;
Canvas* savedCanvas;
Canvas* nextCanvas;
Canvas* scoreCanvas;
Canvas* pauseScreen;
Canvas* startCanvas;

Shape* currentShape;
Shape* previewShape;
Shape* savedShape;

int next[3]; // list containg the next shapes

int paused = 0;
int started = 0;
int score = 0;

Point* fallenCubes[22*10];
int fallCount = 0;

// -- LOGIC FUNCTIONS --

void exitCtrisr(int signal){
    system("clear");
    printf(SHOW_CURSOR);
    //free the canvases
    freeCanvas(c);
    freeCanvas(savedCanvas);
    freeCanvas(nextCanvas);
    freeCanvas(scoreCanvas);
    freeCanvas(pauseScreen);

    //free cubes
    for(int i = 0; i < fallCount; i++){
        free(fallenCubes[i]->color);
        free(fallenCubes[i]);
    }


    freeShape(currentShape);
    freeShape(previewShape);
    freeShape(savedShape);

    exit(0);
}
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
    currentShape = newShape(4,1,next[0]);
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

void updateScore(int rows){
    if(rows == 1) score += 40;
    if(rows == 2) score += 100;
    if(rows == 3) score += 300;
    if(rows == 4) score += 1200;
}

//checks if a row is full if so remove it and make the above go down
int fullRow(){
  int removed = 0;
  for(int i = HEIGHT; i >= 0; i --){
    int count = 0;
    for(int x = 0; x < 10; x++){
      if(isThereAFallenCubeHere(x,i)) count++;
    }
    if(count == 10){
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
      removed++;
      i++;
    }
  }
  return removed;
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
    if(keyInput == 'p'){
      if(!paused){
        system("clear");
        initPixels(pauseScreen);
        setText(pauseScreen, 26/2-7,0,"Paused, press p",WHITE,BG_BLACK);
        setText(pauseScreen, 26/2-5,1,"to unpause",WHITE,BG_BLACK);
        setText(pauseScreen, 0,4,"Hold shape by pressing e",WHITE,BG_BLACK);
        setText(pauseScreen, 26/2-6,6,"Quit with q",WHITE,BG_BLACK);

        draw(pauseScreen);
        setBorder(pauseScreen,2);

        paused = 1;
      }
      else{
        system("clear");
        initPixels(c);
        initPixels(nextCanvas);
        initPixels(savedCanvas);
        initPixels(scoreCanvas);
        paused = 0;
      }
    }
    if(keyInput == 'q'){
        exitCtrisr(0);
    }
    if(keyInput == 'e'){

      if(savedShape->cubes[0].x==-100){
        savedShape = currentShape;
        currentShape = newShape(4,1,1);
      }
      else{
        Shape* temp = savedShape;
        temp->pos.x = currentShape->pos.x;
        temp->pos.y = currentShape->pos.y;
        if(!collides(*temp,newPoint(0,0),fallenCubes,fallCount)){
          savedShape = currentShape;
          currentShape = temp;
        }

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

void initCanvases(){

  c = newCanvas(11,HEIGHT+1,". ",WHITE,BG_BLACK);
  c->x = termWidth()/2-11;
  c->y = termHeight()/2-(HEIGHT/2);

  savedCanvas = newCanvas(6,5,"  ",WHITE,BG_BLACK);
  savedCanvas->x = termWidth()/2-(24);
  savedCanvas->y = termHeight()/2 - (HEIGHT/2);

  nextCanvas = newCanvas(5,4*3+1, "  ", WHITE,BG_BLACK);
  nextCanvas->x = termWidth()/2+12;
  nextCanvas->y = termHeight()/2 - (HEIGHT/2);

  pauseScreen = newCanvas(26,10," ",WHITE,BG_BLACK);
  pauseScreen->x = termWidth()/2-26/2;
  pauseScreen->y = termHeight()/2-(HEIGHT/2);

  scoreCanvas = newCanvas(12,5," ",WHITE,BG_BLACK);
  scoreCanvas->x = termWidth()/2-24;
  scoreCanvas->y = termHeight()/2-(HEIGHT/2)+6 ;

  startCanvas = newCanvas(34,10, " ",WHITE,BG_BLACK);
  startCanvas->x = termWidth()/2-32/2;
  startCanvas->y = termHeight()/2-(HEIGHT/2);
}

void initShapes(){


  srand(time(NULL));
  next[0] = (rand() % (5 - 0 + 1));
  next[1] = (rand() % (5 - 0 + 1));
  next[2] = (rand() % (5 - 0 + 1));


  savedShape = newShape(0,0,1000);
  previewShape = newShape(0,0,0);
  currentShape = newShape(0,0,0);
  currentShape->pos.x = 3;
  currentShape->pos.y = 3;

}
void splashScreen(){
  setCenterText(startCanvas,16,0,"CTRISR",WHITE,BG_BLACK);
  setCenterText(startCanvas,16,1,"Q to quit",WHITE,BG_BLACK);
  setCenterText(startCanvas,16,2,"E to hold",WHITE,BG_BLACK);
  setCenterText(startCanvas,16,3,"W to rotate",WHITE,BG_BLACK);
  setCenterText(startCanvas,16,4,"AD to go sideways",WHITE,BG_BLACK);
  setCenterText(startCanvas,16,5,"SPACE to place at bottom",WHITE,BG_BLACK);
  setCenterText(startCanvas,16,7,"Press any key to start the game",WHITE,BG_BLACK);
  draw(startCanvas);
  setBorder(startCanvas,1);

  msleep(40);
  char temp = getchar();
}

int main(){
  //make so ctr+c will run the exit code
  signal(SIGINT, exitCtrisr);

  initCanvases();
  initShapes();

  splashScreen();

  int tick = 0;
  while(1){
    //check if there is a full row
    updateScore(fullRow());

    if(!paused && tick % 10 == 0){
      currentShape->pos.y++;
    }

    movement(tick);
    setPreviewShape(*currentShape);

    // should not update if game is paused
    if(!paused){
      renderWorld(c,currentShape,previewShape,fallenCubes,fallCount);
      renderScore(scoreCanvas,score);

      renderNext(nextCanvas,next);
      clearPixels(savedCanvas);
      renderSaved(savedCanvas, *savedShape);

    }

    msleep(50);
    tick++;
    //check if the currentShape has fallen down
    for(int i = 0; i < 4; i ++){
        if(currentShape->cubes[i].y+currentShape->pos.y >= HEIGHT ||
           collides(*currentShape,newPoint(0,1),fallenCubes,fallCount)){
          newCurr();
        }
    }
  }

  return 0;
}
