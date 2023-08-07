#!/bin/bash

gcc ./Printer/src/Canvas.c ./Printer/src/msc.c ./src/renderer.c ./src/shapes.c ./src/main.c -lm
read -p "Press Enter to continue" </dev/tty
./a.out


