#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "printer.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

void setFullScreen(Canvas* canvas){
    struct winsize size;

    // Use the ioctl system call to get the terminal size
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1) {
        perror("ioctl");
    }

    int width;
    if(strlen(canvas->bgPixel.ch) > 1)
        width = size.ws_col/2;
    else
        width = size.ws_col;
    int height = size.ws_row;

    canvas->width = width;
    canvas->height = height;
    initPixels(canvas);
   

}

void initPixels(Canvas *canvas){
    if(canvas->pixels != NULL)
        free(canvas->pixels);
    int width = canvas->width;
    int height = canvas->height;


    canvas->pixels = (Pixel*)malloc(sizeof(Pixel)*width*height);

    for(int i = 0; i < canvas->width*height; i ++){
        canvas->pixels[i].ch = canvas->bgPixel.ch;
        canvas->pixels[i].color = canvas->bgPixel.color;
        canvas->pixels[i].bgcolor = canvas->bgPixel.color;
    }

    if(canvas->prevPixels != NULL)
        free(canvas->prevPixels);
    canvas->prevPixels = (Pixel*)malloc(sizeof(Pixel)*width*height);

    for(int i = 0; i < width*height; i ++){
        canvas->prevPixels[i].ch = "";
        canvas->prevPixels[i].color = "";
        canvas->prevPixels[i].bgcolor = "";
    }
    // set bg 
    clearPixels(canvas);
}

Canvas *newCanvas(int width, int height, char* bgCh, char* color, char* bgcolor){
    system("clear");
    disableEcho(); //dont display user input
    Canvas *canvas = malloc(sizeof(Canvas));
    canvas->x = 0;
    canvas->y = 0;
    canvas->bgPixel.ch = bgCh;
    canvas->bgPixel.color = color;
    canvas->bgPixel.bgcolor = bgcolor;
    canvas->width = width;
    canvas->height = height;

    initPixels(canvas);

    //printf("\033[?25h");
    return canvas;
}
Pixel *newPixel(int x, int y, char* ch, char* color,char* bgcolor){
    Pixel *pixel = (Pixel*)malloc(sizeof(Pixel));
    pixel->x = x;
    pixel->y = y;
    pixel->ch = ch;
    pixel->color = color;
    pixel->bgcolor = bgcolor;

    return pixel;
}

void freeStrings(Canvas *canvas){
    for(int i = 0 ; i < canvas->numStrings;i++){
        free(canvas->strings[i]);
    }
    canvas->numStrings = 0;
}

void freeCanvas(Canvas *canvas){
    printf("\033[?25h");
    freeStrings(canvas);
    free(canvas->strings);
    free(canvas->pixels);
    free(canvas->prevPixels);
    free(canvas);
}

void draw(Canvas *canvas){

    if(canvas->prevPixels != NULL){

        int i = 0;
        int bgSize = strlen(canvas->bgPixel.ch);
        //for unicode it gets fucked otherwise if a unicode is looking like 1 it is actually 4 
        bgSize = bgSize > 1 ? 2 : 1;
        for(int y = 0; y < canvas->height;y++){
            for(int x = 0; x < canvas->width*bgSize;x+=bgSize){
                Pixel p = canvas->pixels[i];
                if(strcmp(canvas->pixels[i].ch, canvas->prevPixels[i].ch) != 0 || strcmp(canvas->pixels[i].color, canvas->prevPixels[i].color) != 0){
                    char *print = malloc(sizeof(char*)*10);
                    sprintf(print, "%s%s%s%s%s",p.color,p.bgcolor, p.ch,HIDE_CURSOR,RESET);
                    setCharAt(x+canvas->x,y+canvas->y,print);

                    free(print);

                    fflush(stdout); 
                    setCursorPosition(canvas->width+canvas->x, canvas->height+canvas->height);
                    canvas->prevPixels[i] = canvas->pixels[i];
                }
                i++;
            }
        }
    }    
}

void setRect(Canvas *canvas,Rect *rect){
    int _x = rect->x;
    int _y = rect->y;
    int w = rect->w;
    int h = rect->h;
    char *ch = rect->ch;
    char *color = rect->color;
    char *bgcolor = rect->bgcolor;


    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            setPixel(canvas, _x+x, _y+y, ch, color, bgcolor);
        }
    }

}

void setCircle(Canvas *canvas, int _x, int _y, int radius, char* ch, char* color, char* bg){
   
        for(int i = 1; i < 360; i++){
            int x = (int) radius * cos(i);
            int y = (int) radius * sin(i);
            setPixel(canvas,_x+x,_y+y,ch,color,bg);
        }
}

void fillCircle(Canvas *canvas, int _x, int _y, int radius, char* ch, char* color, char* bg){
    while(radius > 1){
        setCircle(canvas,_x,_y,radius,ch,color,bg);
        radius--;
    }
}

void setCursorPosition(int x, int y) {
    printf("\033[%d;%dH", y+1, x+1);
}

void setCharAt(int x, int y, char *c) {
    setCursorPosition(x, y);
    printf("%s",c);
}
void setRender(Canvas *canvas){

}
void setPixel(Canvas *canvas, int _x, int _y, char* ch, char* color, char* bgcolor){
    setPixelRaw(canvas,_x, _y , ch,color,bgcolor);
}


void setPixelRaw(Canvas *canvas, int _x, int _y, char* ch, char* color, char* bgcolor){
    int index = 0;
    for(int y = 0; y < canvas->height; y++){
        for(int x = 0; x < canvas->width; x++){
            if(_x == x && _y == y){
                canvas->pixels[index].ch = ch;
                canvas->pixels[index].color = color;
                canvas->pixels[index].bgcolor = bgcolor;
            }
            index++;
        }
    }
}

Pixel *getPixel(Canvas *canvas, int _x, int _y){
    int index = 0;
    for(int y = 0; y < canvas->height; y++){
        for(int x = 0; x < canvas->width; x++){
            if(_x == x && _y == y){
                return &canvas->pixels[index];
            }
            index++;
        }
    }
    return NULL;
}


void setBgOfPixel(Canvas *canvas, int x, int y, char* bgcolor){
    getPixel(canvas, x, y);
}

void setPixelWithPixel(Canvas *canvas, Pixel *pixel){
    setPixel(canvas, pixel->x, pixel->y, pixel->ch, pixel->color, pixel->bgcolor);
}



void clearPixels(Canvas *canvas){
    //free the strings saved 
    freeStrings(canvas);

    int index = 0;
    for(int y = 0; y < canvas->height; y++){
        for(int x = 0; x < canvas->width; x++){
            canvas->pixels[index].ch = canvas->bgPixel.ch;
            canvas->pixels[index].color = canvas->bgPixel.color;
            canvas->pixels[index].bgcolor = canvas->bgPixel.bgcolor;
            index++;
        }
    }
}

void addString(Canvas *canvas, char *newStr){
    canvas->strings = (char**)realloc(canvas->strings, (canvas->numStrings+1)*sizeof(char*));
    (canvas->strings)[canvas->numStrings] = newStr;
    canvas->numStrings++;
}

void setText(Canvas *canvas, int _x, int _y, char* text, char* color, char* bgcolor){
    int len = strlen(text);
    for(int i = 0; i < len; i++){
        if(strlen(canvas->bgPixel.ch) > 1){
            char *newStr = malloc(sizeof(char)*3);
            newStr[0] = text[i];
            newStr[1] = ' ';
            newStr[2] = '\0';
            addString(canvas, newStr);
            setPixel(canvas, _x+i, _y, newStr, color, bgcolor);
        }
        else{
            char *newStr = malloc(sizeof(char)*2);
            newStr[0] = text[i];
            newStr[1] = '\0';
            addString(canvas, newStr);
            setPixel(canvas, _x+i, _y, newStr, color, bgcolor);
        }

    }
}

void setCenterText(Canvas *canvas, int _x, int _y, char* text, char* color, char* bgcolor){
    int size = strlen(text)/2;
    setText(canvas,_x-size,_y,text,color,bgcolor);
}

void setBorder(Canvas* canvas, int borderWidth){
  drawBorder(canvas,borderWidth);
}

void drawBorder(Canvas *canvas, int borderWidth){

    char* rightBorder = "┃";
    char* leftBorder = "┃";
    char* bottom = "━━";
    char* top = "━━";
    char* joinTopRight = "┏━";
    char* joinBottomRight = "┗━";
    char* joinTopLeft = "┓";
    char* joinBottomLeft = "┚";

    int scale = strlen(canvas->bgPixel.ch);

    // vertical lines
    for(int i = 0; i < canvas->height; i ++){
        setCharAt(canvas->x-1,i+canvas->y,rightBorder);
        setCharAt(canvas->width*scale+canvas->x,i+canvas->y,rightBorder);
    }
    // horzontal lines
    for(int i = 0; i < canvas->width*scale; i +=scale){
        setCharAt(i+canvas->x,canvas->y-1,top);
        setCharAt(i+canvas->x,canvas->height+canvas->y,bottom);
    }
    setCharAt(canvas->x-1,canvas->y-1,joinTopRight);
    setCharAt(canvas->x-1,canvas->height+canvas->y,joinBottomRight);
    setCharAt(canvas->width*scale+canvas->x, canvas->y-1,joinTopLeft);
    setCharAt(canvas->width*scale+canvas->x, canvas->height+canvas->y,joinBottomLeft);


}
struct winsize win;

unsigned int termWidth(){
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
    return (win.ws_col);
}
unsigned int termHeight(){
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
    return (win.ws_row);
}
