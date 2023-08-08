#include "renderer.h"
#include "../Printer/src/Canvas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Canvas *scoreCanvas;
Canvas * canvas;
Canvas* getCurrentCanvas() {
    return canvas;
}
Canvas* getScoreCanvas() {
    return scoreCanvas;
}

void initRenderer(){
    int width = 12;
    canvas = newCanvas(width,22,"  ",BLACK,BG_BLACK);

    canvas->x = termWidth()/2-width/2;
    canvas->y = 12;

    scoreCanvas = newCanvas(width,10,"  ",RED,BG_BLACK);
    scoreCanvas->y = 2;

    scoreCanvas->x = termWidth()/2-width/2;
}

void renderCubeChar(Point p, int x, int y, char* car){
    setPixel(canvas,p.x+x,p.y+y,car,p.color,BG_BLACK);
}
void renderCube(Point p, int x, int y){
    renderCubeChar(p,x,y,"🮔 ");
}

void render(Shape* currentShape, Shape* previewShape, Point  *fallenCubes, int fallenCount){
    if(canvas == NULL){
        initRenderer();
    }
    
    clearPixels(canvas);
    //print the walls
    for(int i = 0; i < canvas->height; i++){
        setPixel(canvas,0 ,i,"🮔 ",WHITE,BG_BLACK);
        setPixel(canvas,canvas->width-1,i,"🮔 ",WHITE,BG_BLACK);
    }
    for(int i = 0; i < canvas->width; i++){
        setPixel(canvas,i ,canvas->height-1,"🮔 ",WHITE,BG_BLACK);
    }

    for(int i = 0; i < 4; i++){
        Point cube = currentShape->cubes[i];

        int x = currentShape->pos.x;
        int y = currentShape->pos.y;
        renderCube(cube, x, y);
    }
    for(int i = 0; i < 4; i++){

        Point pcube = currentShape->cubes[i];

        int x = previewShape->pos.x;
        int y = previewShape->pos.y;
        renderCube(pcube,x,y);
    }                                                        
    for(int j = 0; j < fallenCount; j++){
        Point cube = fallenCubes[j];

        renderCube(cube,0,0);
    }
    

    draw(canvas);
}

void renderScore(int score, Shape* savedShape){
    
    clearPixels(scoreCanvas);
    for(int i = 0; i < scoreCanvas->height; i++){
        setPixel(scoreCanvas,0 ,i,"🮔 ",WHITE,BG_BLACK);
        setPixel(scoreCanvas,scoreCanvas->width-1,i,"🮔 ",WHITE,BG_BLACK);
    }
    for(int i = 0; i < scoreCanvas->width; i++){
        setPixel(scoreCanvas,i ,scoreCanvas->height-1,"🮔 ",WHITE,BG_BLACK);
    }

    char *scoreStr= malloc(sizeof(char*));
    sprintf(scoreStr, "S%d", score);
    setText(scoreCanvas,scoreCanvas->width/2-strlen(scoreStr)/2,1,scoreStr,WHITE,BG_BLACK);
    free(scoreStr);

    if(savedShape != NULL){
        for(int i = 0; i < 4; i++){
            Point cube = savedShape->cubes[i];

            int x = 5;
            int y = 4;
            setPixel(scoreCanvas,cube.x+x,cube.y+y,"🮔 ",cube.color,BG_BLACK);
        }
    }

    draw(scoreCanvas);
}
