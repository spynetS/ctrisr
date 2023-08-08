#include "shapes.h"
#include "msc.c"
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"

#define WIDTH 11
#define HEIGHT 22


int paused = 0;
Point fallenCubes[1000]; // all places 
int fallenCount = 0; //keep count of how many has fallen
int score       = 0; 
int fallDelay   = 20; // how long to wait to fall
int fallDeleyDecreser = 500; // when zero decrese fallDelay
struct winsize w;

Shape *currentShape;
Shape *previewShape;
Shape *savedShape;


void end(){
    printf("\nYou got %d\n",score);
    printf("\e[?25h");
    exit(0);
}
void center(){
    for(int i = 0; i< w.ws_col/2-WIDTH; i++){
        printf(" ");
    }
}

void startScreen(){
    system("clear");
    for(int i = 0; i< w.ws_row/2-5; i++){
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



void renderWorld(Shape *currentShape){


    render(currentShape,previewShape, fallenCubes, fallenCount); 
    renderScore(score,savedShape); 
    // system("clear");
    // if(paused){
    //     center();
    //     printf("Press p to continue");
    // }
    // renderScore();
    //
    // for(int y = 0; y < HEIGHT; y++){
    //     center();
    //     printf("█ ");
    //     for(int x = 0; x < WIDTH; x++){
    //         // if 1 dont print . char 
    //         int at = 0;
    //         //render current shape
    //         for(int i = 0; i < 4; i++){
    //             Point cube = currentShape->cubes[i];
    //             Point pcube = previewShape->cubes[i];
    //             if( currentShape->pos.x+cube.x == x &&
    //                     currentShape->pos.y+cube.y == y){
    //                 renderPoint(cube);
    //                 at = 1;
    //             }
    //         }
    //         if(!at){
    //             for(int i = 0; i < 4; i++){
    //                 Point cube = currentShape->cubes[i];
    //                 if( previewShape->pos.x+cube.x == x &&
    //                         previewShape->pos.y+cube.y == y){
    //                     renderPointChar(cube,"[]");
    //                     at = 1;
    //                 }
    //             }
    //
    //         }
    //         for(int j = 0; j < fallenCount; j++){
    //             Point cube = fallenCubes[j];
    //             if( cube.x == x &&
    //                     cube.y == y){
    //                 renderPoint(cube);
    //                 at = 1;
    //             }
    //         }
    //         if(!at)
    //             printf("· ");
    //     }
    //     printf("█\n");
    // }
    // center();
    // printf("███████████████████████\n");
    // printf("\e[?25l");
}

void setPreview(Shape shape){
    // loop though all cubes 
    for(int i = 0; i < 4; i++){
        //check if it collides att bottom and upwards
        for(int r = 0; r < HEIGHT-currentShape->pos.y; r++){
            // if collides set the previewShape position
            if(collides(*currentShape,newPoint(0, r+1), fallenCubes,fallenCount)){
                previewShape = newShape(shape.pos.x, currentShape->pos.y+r, shape.type);
                return;
            }
        }
    }
}
int removeFullRow(){
    int count = 0; // amount of cubes on a row
    int rows = 0; // rows that are full
    //begin from bottom (want to remove bottom up)
    for(int r = HEIGHT; r >= 0; r--){
        count = 0;
        //count amount on row 
        for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
            if(fallenCubes[c].y == r){
                count++;
            }
        }
        if(count == 10){
            //remove chars
            rows++;
            for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
                if(fallenCubes[c].y == r){
                    // should free this value instead of move
                    fallenCubes[c].y = 100;
                    renderWorld(currentShape);
                    msleep(30);
                }
            }
            //move above down
            for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
                if(fallenCubes[c].y < r){
                    fallenCubes[c].y++; 
                }
                //renderWorld(currentShape);
            }
            r++;
        }
    }
    setPreview(*currentShape);
    return rows;
}

void updateScore(int rows){
    if(rows == 1) score += 40;
    if(rows == 2) score += 100;
    if(rows == 3) score += 300;
    if(rows == 4) score += 1200;
}

void setNewShape(){
    // get random number (shape)
    srand(time(0));
    int number = (rand() % (5 - 0 + 1)) + 0;
    currentShape = newShape(4,-1,number);
    previewShape = newShape(4,-1,number);
    setPreview(*currentShape); 
}

// draw full blocks where preview was to f;ash it
void flashPreview(){
    for(int i = 0; i < 4; i++){
        Point cube = currentShape->cubes[i];

        int x = currentShape->pos.x;
        int y = currentShape->pos.y;
        renderCubeChar(cube, x, y,"██");
    }
    draw(getCurrentCanvas());
    msleep(200);

}

int main(){
    
    // retrive terminal width and height
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); 
   
    startScreen();

    //create current shape
    currentShape = newShape(5,1,2);
    //create a placed preview
    previewShape = newShape(1,16,0);

    int renderTime = 0; 
    while(1){
        msleep(50);
        if(fallDeleyDecreser == 0){
            fallDelay --;
            fallDeleyDecreser = 500;
        }
        fallDeleyDecreser--;
        //amount of rows deleted addes score
        updateScore(removeFullRow());


        //key pressed?
        if(kbhit()){
            //get key
            char key = getchar();
            if(key == 'q'){
                end();
            }
            if(key == 'e'){
                //save shape
                if(savedShape == NULL){
                    savedShape = currentShape;
                    setNewShape();
                }
                //retrive
                else{
                    Shape *temp = currentShape;
                    currentShape = savedShape;
                    currentShape->pos.y = temp->pos.y;
                    currentShape->pos.x = temp->pos.x;
                    savedShape = temp;
                }
            }
            if(key == 'p'){
                paused = paused==1?0:1;
            }
            if(key == 'a' && !collides(*currentShape,newPoint(-1,0),&fallenCubes[0],fallenCount) && !paused){
                currentShape->pos.x--;
            }
            if(key == 's' && !collides(*currentShape,newPoint(0,1),&fallenCubes[0],fallenCount)&& !paused){
                //fallDelay = 0;
                currentShape->pos.y ++;
                score++;
            }
            if(key == ' ' && !collides(*currentShape,newPoint(0,1),&fallenCubes[0],fallenCount)&& !paused){
                //fallDelay = 0;
                currentShape->pos.y = previewShape->pos.y;
                
                flashPreview();

                score++;
            }
            if(key == 'd' && !collides(*currentShape,newPoint(1,0),&fallenCubes[0],fallenCount)&& !paused){
                currentShape->pos.x++;
            }
            else if(key == 'w'&& !paused){
                // if shape collides when rotate move it either right or left
                for(int i = 0; i < 4; i++){ // 4 is max length
                    if(rotateCollide(*currentShape, &fallenCubes[0], fallenCount)){
                        if(currentShape->pos.x < 5 && !collides(*currentShape, newPoint(1,0),&fallenCubes[0],fallenCount) ){
                            currentShape->pos.x++;
                        }else if(!collides(*currentShape, newPoint(-1,0),&fallenCubes[0],fallenCount)){
                            currentShape->pos.x--;
                        }
                    }else{
                        rotate(currentShape);
                        break;
                    }
                }
                // then rotate
            }

            //calculate where places preview should be
            setPreview(*currentShape); 
            renderWorld(currentShape);
        }
        if(!paused){

            // move shape down
            if(!collides(*currentShape, newPoint(0,1), &fallenCubes[0], fallenCount) ){
                if(renderTime > fallDelay){
                    currentShape->pos.y++;
                    renderWorld(currentShape);
                    renderTime =0;
                }
            }else{
                //end
                if(currentShape->pos.y == -1){
                    end();
                }
                //add cubes to fallen array and add shape pos to them
                for(int i = 0; i < 4; i++){
                    Point newCube = currentShape->cubes[i];
                    newCube.x += currentShape->pos.x;
                    newCube.y += currentShape->pos.y;
                    fallenCubes[fallenCount] = newCube;
                    fallenCount++;
                }
                setNewShape();
                score++;
            }
            renderTime++;
        }
    }

    end();
}

