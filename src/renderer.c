#include "renderer.h"
#include "../Printer/src/Canvas.h"
#include "shapes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "sound.h"

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
    canvas = newCanvas(width,22,"  ",WHITE,BG_BLACK);
 
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
    renderCubeChar(p,x,y,cubeStr); 
}

void render(Shape* currentShape, Shape* previewShape, Point  *fallenCubes, int fallenCount){
    if(canvas == NULL){
        initRenderer();
    }
    
    clearPixels(canvas);
    //print the walls
    for(int i = 0; i < canvas->height; i++){
        setPixel(canvas,0 ,i,cubeStr,CYAN,BG_BLACK);
        setPixel(canvas,canvas->width-1,i,cubeStr,CYAN,BG_BLACK);
    }
    for(int i = 0; i < canvas->width; i++){
        setPixel(canvas,i ,canvas->height-1,cubeStr,CYAN,BG_BLACK);
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
        setPixel(scoreCanvas,0 ,i,cubeStr,CYAN,BG_BLACK);
        setPixel(scoreCanvas,scoreCanvas->width-1,i,cubeStr,CYAN,BG_BLACK);
    }
    for(int i = 0; i < scoreCanvas->width; i++){
        setPixel(scoreCanvas,i ,scoreCanvas->height-1,cubeStr,CYAN,BG_BLACK);
        setPixel(scoreCanvas,i ,0,cubeStr,CYAN,BG_BLACK);
    }

    char *scoreStr= malloc(sizeof(char*));
    sprintf(scoreStr, "S%d", score);
    setText(scoreCanvas,scoreCanvas->width/2-strlen(scoreStr)/2,1,scoreStr,CYAN,BG_BLACK);
    free(scoreStr);

    if(savedShape != NULL){
        for(int i = 0; i < 4; i++){
            Point cube = savedShape->cubes[i];

            int x = 5;
            int y = 4;
            setPixel(scoreCanvas,cube.x+x,cube.y+y,cubeStr,cube.color,BG_BLACK);
        }
    }

    draw(scoreCanvas);
}

void destroyCube(Point cube){
    setPixel(getCurrentCanvas(), cube.x, cube.y, destroyCubeStr1,cube.color,BG_BLACK);
    draw(getCurrentCanvas());
    msleep(10);

    setPixel(getCurrentCanvas(), cube.x, cube.y, destroyCubeStr2,cube.color,BG_BLACK);
    draw(getCurrentCanvas());
    msleep(10);

    setPixel(getCurrentCanvas(), cube.x, cube.y, destroyCubeStr3,cube.color,BG_BLACK);
    draw(getCurrentCanvas());
    msleep(10);
}
// draw full blocks where preview was to f;ash it
void flashPreview(Shape *currentShape){
    note('c',300,1);
    note('E',100,1);

    for(int i = 0; i < 4; i++){
        Point cube = currentShape->cubes[i];

        int x = currentShape->pos.x;
        int y = currentShape->pos.y;
        renderCubeChar(cube, x, y,flashCubeStr);
    }
    draw(getCurrentCanvas());
    msleep(200);

}
void end(int score){
    printf("\nYou got %d\n",score);
    printf("\e[?25h");
    exit(0);
}
void center(){
    for(int i = 0; i< termWidth()/2-22; i++){
        printf(" ");
    }
}

void startScreen(){
    system("clear");
    for(int i = 0; i< termHeight()/2-5; i++){
        printf("\n");
    }
    center();
    printf("          CTRISR\n\n");
    center();
    printf("         q to quit\n");
    center();
    printf("         p to pause\n");
    center();
    printf("         e to save\n");
    center();
    printf("   'a d' move side ways\n");
    center();
    printf("'w' rotate, 's' to move down\n\n");
    center();
    printf("    Press any key to start\n");
    center();
    printf("\e[?25l");
    system("stty raw");
    getchar();
    system("stty cooked");
}
