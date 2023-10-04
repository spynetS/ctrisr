#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Canvas.h"

int main() {
   system("clear");
   Canvas *canvas = newCanvas(10,10,"##",RED,BG_BLACK);
   setBorder(canvas, 0);

   while(1){
      draw(canvas); 
      clearPixels(canvas);

      printer_msleep(10);
   }
   

   return 0;
}
