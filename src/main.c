#include "shapes.c"
#include <time.h>
#include <errno.h>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define WIDTH 10
#define HEIGHT 20

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
  
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  
  ch = getchar();
  
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  
  if(ch != EOF)
  {
      ungetc(ch, stdin);
      return 1;
  }
  
  return 0;
}

Point fallenCubes[WIDTH*HEIGHT]; // all places 
int fallenCount = 0; //keep count of how many has fallen
int score       = 1; 
int fallDelay   = 6; // how long to wait to fall

struct winsize w;

Shape *currentShape;
Shape *previewShape;


int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void center(){
    for(int i = 0; i< w.ws_col/2-WIDTH; i++){
        printf(" ");
    }
}

void startScreen(){
    system("clear");
    center();
    printf("Tetris\n");
    center();
    printf("a d move side ways\n");
    center();
    printf("w rotate s to move down\n");
    center();
    printf("Press any key to start\n");
    center();
    printf("\e[?25l");
    getchar();
}

void renderScore(){
    center();
    printf("███████████████████████\n");
    center();
    printf("█         Score       █\n");

    center();
    if(score < 10){
        printf("█          %d          █\n",score);
    }
    else if(score >= 100)
    {
        printf("█         %d         █\n",score);
    }
    else if(score >= 10)
    {
        printf("█          %d         █\n",score);
    }
    center();
    printf("█                     █\n");
    center();
    printf("███████████████████████\n");
}


void renderWorld(Shape *currentShape){
    
    system("clear");
    renderScore();

    for(int y = 0; y < HEIGHT; y++){
        center();
        printf("█ ");
        for(int x = 0; x < WIDTH; x++){
            // if 1 dont print . char 
            int at = 0;
            //render current shape
            for(int i = 0; i < 4; i++){
                Point cube = currentShape->cubes[i];
                Point pcube = previewShape->cubes[i];
                if( currentShape->pos.x+cube.x == x &&
                        currentShape->pos.y+cube.y == y){
                    renderPoint(cube);
                    at = 1;
                }
            }
            if(!at){
                for(int i = 0; i < 4; i++){
                    Point cube = currentShape->cubes[i];
                    if( previewShape->pos.x+cube.x == x &&
                            previewShape->pos.y+cube.y == y){
                        renderPointChar(cube,'=');
                        at = 1;
                    }
                }

            }
            for(int j = 0; j < fallenCount; j++){
                Point cube = fallenCubes[j];
                if( cube.x == x &&
                        cube.y == y){
                    renderPoint(cube);
                    at = 1;
                }
            }
            if(!at)
                printf(". ");
        }
        printf("█\n");
    }
    center();
    printf("███████████████████████\n");
    printf("\e[?25l");
}

void setPreview(Shape shape){
    
    // loop though all cubes 
    for(int i = 0; i < 4; i++){
        //check if it collides att bottom and upwards
        for(int r = 0; r < 20-currentShape->pos.y; r++){
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
    for(int r = 20; r >= 0; r--){
        count = 0;
        //count amount on row 
        for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe if(fallenCubes[c].y == r){
                count++;
            }
        }
        if(count == 10){
            //remove chars
            rows++;
            for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
                if(fallenCubes[c].y == r){
                    // should free this value instead of move
                    renderWorld(currentShape);
                    fallenCubes[c].y = 100;
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
    return rows;
}

int main(){
    
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); 
    startScreen();

    //create current shape
    currentShape = newShape(5,1,2);
    //create a placed preview
    previewShape = newShape(1,16,0);

    int renderTime = 0; 

    while(1){
        msleep(50);
        
        score += removeFullRow()*10;

        //calculate where places preview should be
        setPreview(*currentShape); 
        renderWorld(currentShape);
        
        //key pressed?
        if(kbhit()){
            //get key
            char key = getchar();
            if(key == 'c'){
                break;
            }
            if(key == 'a' && !collides(*currentShape,newPoint(-1,0),&fallenCubes[0],fallenCount)){
                currentShape->pos.x--;
            }
            if(key == 's' && !collides(*currentShape,newPoint(0,1),&fallenCubes[0],fallenCount)){
                //fallDelay = 0;
                currentShape->pos.y ++;
            }else{
                fallDelay = 6;
            }
            if(key == 'd' && !collides(*currentShape,newPoint(1,0),&fallenCubes[0],fallenCount)){
                currentShape->pos.x++;
            }
            else if(key == 'w'){
                // if shape collides when rotate move it either right or left
                while(rotateCollide(*currentShape)){
                    if(currentShape->pos.x < 5){
                        currentShape->pos.x++;
                    }else{
                        currentShape->pos.x--;
                    }
                }
                // then rotate
                rotate(currentShape);
            }
        }
        // move shape down
        if(!collides(*currentShape, newPoint(0,1), &fallenCubes[0], fallenCount) ){
            if(renderTime > fallDelay){
                currentShape->pos.y++;
                renderTime =0;
            }
        }else{
            //add cubes to fallen array and add shape pos to them
            for(int i = 0; i < 4; i++){
                Point newCube = currentShape->cubes[i];
                newCube.x += currentShape->pos.x;
                newCube.y += currentShape->pos.y;
                fallenCubes[fallenCount] = newCube;
                fallenCount++;
            }
            // get random number (shape)
            int number = (rand() % (5 - 0 + 1)) + 0;
            currentShape = newShape(5,1,number);
            previewShape = newShape(5,1,number);
        }
        renderTime++;
    }
    printf("\e[?25h");
    return 0;
}

