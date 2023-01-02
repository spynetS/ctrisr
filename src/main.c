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

int paused = 0;
struct node *fallenCubes; // all places 
int score       = 0; 
int fallDelay   = 12; // how long to wait to fall

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
    for(int i = 0; i< w.ws_row/2-5; i++){
        printf("\n");
    }
    center();
    printf("           Tetris\n\n");
    center();
    printf("         q to quit\n");
    center();
    printf("         p to pause\n");
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

void renderScore(){
    for(int i = 0; i< (w.ws_row-HEIGHT)/4; i++){
        printf("\n");
    }
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
int fallenC = 0;

void renderWorld(Shape *currentShape){
    
    system("clear");
    if(paused){
        center();
        printf("Press p to continue");
    }
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
                        renderPointChar(cube,"[]");
                        at = 1;
                    }
                }

            }
            int fallenCount = listLen(fallenCubes);
            for(int j = 0; j < fallenCount; j++){
                Point *cube = (Point*)get(fallenCubes,j)->value;
                if(cube->x == x &&
                        cube->y == y){
                    printf("x ");
                    //renderPoint(*cube);
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
            if(collides(*currentShape,newPoint(0, r+1), fallenCubes)){
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
        int fallenCount = listLen(fallenCubes);
        for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
            if(((Point *)get(fallenCubes,c)->value)->y == r){
                count++;
            }
        }
        if(count == 10){
            //remove chars
            rows++;
            for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
                if(((Point *)get(fallenCubes,c)->value)->y == r){
                    // should free this value instead of move
                    renderWorld(currentShape);
                    ((Point *)get(fallenCubes,c)->value)->y = 100;
                    msleep(30);
                }
            }
            //move above down
            for(int c = 0; c < fallenCount; c++){ // hehe c++ hehe
                if(((Point *)get(fallenCubes,c)->value)->y < r){
                    ((Point *)get(fallenCubes,c)->value)->y++; 
                }
                //renderWorld(currentShape);
            }
            r++;
        }
    }
    return rows;
}

void updateScore(int rows){
    if(rows == 1) score += 40;
    if(rows == 2) score += 100;
    if(rows == 3) score += 300;
    if(rows == 4) score += 1200;
}

int main(){
    
    // retrive terminal width and height
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); 
   
    startScreen();
    fallenCubes = init_list();
    //create current shape
    currentShape = newShape(5,1,2);
    //create a placed preview
    previewShape = newShape(1,16,0);

    int renderTime = 0; 
    while(1){

        msleep(50);
        
        updateScore(removeFullRow());


        //calculate where places preview should be
        setPreview(*currentShape); 
        renderWorld(currentShape);
         
        //key pressed?
        if(kbhit()){
            //get key
            char key = getchar();
            if(key == 'q'){
                return 0;
            }
            if(key == 'p'){
                paused = paused==1?0:1;
            }
            if(key == 'a' && !collides(*currentShape,newPoint(-1,0),fallenCubes) && !paused){
                currentShape->pos.x--;
            }
            if(key == 's' && !collides(*currentShape,newPoint(0,1),fallenCubes)&& !paused){
                //fallDelay = 0;
                currentShape->pos.y ++;
            }
            if(key == 'd' && !collides(*currentShape,newPoint(1,0),fallenCubes)&& !paused){
                currentShape->pos.x++;
            }
            else if(key == 'w'&& !paused){
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
        if(!paused){

            // move shape down
            if(!collides(*currentShape, newPoint(0,1), fallenCubes) ){
                if(renderTime > fallDelay){
                    currentShape->pos.y++;
                    renderTime =0;
                }
            }else{
                //add cubes to fallen array and add shape pos to them
                for(int i = 0; i < 4; i++){
                    Point *newCube = malloc(sizeof (Point));
                    newCube->x = currentShape->cubes[i].x;
                    newCube->y = currentShape->cubes[i].y;

                    newCube->x += currentShape->pos.x;
                    newCube->y += currentShape->pos.y;
                    printf("%d\n",listLen(fallenCubes));
                    push(fallenCubes,newCube);
                }
                // get random number (shape)
                srand(time(0));
                int number = (rand() % (5 - 0 + 1)) + 0;
                currentShape = newShape(5,0,number);
                previewShape = newShape(5,0,number);
            }
            renderTime++;
        }
    }

    return 0;
}

