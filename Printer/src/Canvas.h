#pragma once


/*BG_BLACK    (Alfred Roos 2023-07-29)
    USE AS IS
    This library makes printing to the termninal alot easier.
    Instead of fomrating your prints yourself with a lot of for
    loops and such with this library you could just set the 'pixels'
    of your canvas to whatever you want.
    
    example:
    //creating a new canvas 20 width and 20 height
    canvas *canvas = newcanvas(20,20,'.',black);
    //set pixel at 10,10 to @ with color green
    setpixel(canvas, 10,10,'@', green);
    // draw the canvas
    draw(canvas);
    //dont forget to free the canvas with the freecanvas function
    // it frees everything the canvas has allocated
    freecanvas(canvas);

*/


#define BG_BLACK  "\x1b[40m"
#define BG_RED    "\x1b[41m"
#define BG_GREEN  "\x1b[42m"
#define BG_YELLOW "\x1b[43m"
#define BG_BLUE   "\x1b[44m"
#define BG_PURPLE "\x1b[45m"
#define BG_CYAN   "\x1b[46m" 
#define BG_WHITE  "\x1b[47m"


#define BLACK  "\x1b[30m"
#define RED    "\x1b[31m"
#define GREEN  "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE   "\x1b[34m"
#define PURPLE "\x1b[35m"
#define CYAN   "\x1b[36m" 
#define WHITE  "\x1b[37m"
#define BG     "██"

#define RESET "\x1b[0m"
#define BG_RESET "\x1b[10m"

#define NOCURSOR "\033[?25l"

/** a struct to hold data a pixel needs */
typedef struct pixel
{
    int x;
    int y;
    char* ch;
    char* color;
    char* bgcolor;

}Pixel;


/** 
The canvas to set pixels to print to stdout 

if the canvas ch property is one char every other char will also be one
if it is 2 all the others will also be 2.
if they are set only to one size the next will be a space.

when you use 2 chars one pixel will appear square because a
char is 2 tall and 1 width

usage:

    //creating a new canvas 20 width and 20 height
    canvas *canvas = newcanvas(20,20,'.',black);
    //set pixel at 10,10 to @ with color green
    setpixel(canvas, 10,10,'@', green);
    // draw the canvas
    draw(canvas);
    //dont forget to free the canvas with the freecanvas function
    // it frees everything the canvas has allocated
    freecanvas(canvas);

prints:
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . @ . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 
    . . . . . . . . . . . . . . . . . . . . 

*/
typedef struct canvas
{
    int x;
    int y;
    int width;
    int height;
    Pixel bgPixel;
    Pixel *pixels;
    Pixel *prevPixels;
    char** strings;
    int numStrings;
    char* render;

}Canvas;

typedef struct rect{
    int x;
    int y;
    int w;
    int h;
    char* ch;
    char* color;
    char* bgcolor;
    

}Rect;

void initPixels(Canvas *canvas);
/**  */
void setCursorPosition(int x, int y);

void setCharAt(int x, int y, char *c);

void setRect(Canvas *canvas, Rect *rect);
void setCircle(Canvas *canvas, int _x, int _y, int radius, char* ch, char* color, char* bg);
void fillCircle(Canvas *canvas, int _x, int _y, int radius, char* ch, char* color, char* bg);
//canvas.c
void setRender(Canvas *canvas);
/** Sets all pixelts to default pixels (clears all set) */
void clearPixels(Canvas *canvas);
/** sets the pixel at x,y with the data */
void setPixel(Canvas *canvas, int _x, int _y, char* ch, char* color, char* bgcolor);
/** set the pixel with the pixel arg */
void setPixelWithPixel(Canvas *canvas, Pixel *pixel);
/** Sets the text at begining of x and at row y */
void setText(Canvas *canvas, int _x, int _y, char* text, char* color, char* bgcolor);

void setBgOfPixel(Canvas *canvas, int x, int y, char* bgcolor);
/** Internal function. Adds string to a list so we can free it later */
void addString(Canvas*, char *newStr);

void setFullScreen(Canvas* canvas);
/** prints all the pixels set in the canvas (prints with a space after to make it appear square)*/
void draw(Canvas *canvas);
/** frees the canvas */
void freeCanvas(Canvas *canvas);
/** returns a new pixel with the data */
Pixel *newPixel(int x, int y, char* ch, char* color, char* bgcolor);
/** returns new canvas with the data */
Canvas *newCanvas(int width, int height, char* bgCh, char* color, char* bgcolor);

//mcs.c
int kbhit(void); //returns 1 if key was pressed
char getKeyPressed(); // returns char if it was pressed otherwise [
int msleep(long msec);
// Function to disable terminal echoing
void disableEcho();
// Function to enable terminal echoing
void enableEcho();

unsigned int termWidth();
unsigned int termHeight();

