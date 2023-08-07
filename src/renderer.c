#include "renderer.h"
#include "../printer/src/Canvas.h"
#include <stdio.h>

Canvas* canvas;
void initRenderer(){
    canvas = newCanvas(12,22,"| ",BLACK,BG_BLACK);
}

void renderCube(Point p, int x, int y){
    setPixel(canvas,p.x+x,p.y+y,"🮔",p.color,BG_BLACK);
}

void render(Shape* currentShape, Shape* previewShape, Point  *fallenCubes, int fallenCount){
    if(canvas == NULL) initRenderer();
    
    clearPixels(canvas);
    //print the walls
    for(int i = 0; i < canvas->height; i++){
        setPixel(canvas,0 ,i,"🮔",WHITE,BG_BLACK);
        setPixel(canvas,canvas->width-1,i,"🮔",WHITE,BG_BLACK);
    }
    for(int i = 0; i < canvas->width; i++){
        setPixel(canvas,i ,canvas->height-1,"🮔",WHITE,BG_BLACK);
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
        setPixel(canvas,pcube.x+x,pcube.y+y,"🮔",pcube.color,BG_BLACK);
    }                                                        
    for(int j = 0; j < fallenCount; j++){
        Point cube = fallenCubes[j];

        renderCube(cube,0,0);
    }
    

    draw(canvas);
}



