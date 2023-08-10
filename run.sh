#!/bin/bash

gcc ./Printer/src/Canvas.c ./Printer/src/msc.c ./src/renderer.c ./src/shapes.c  ./src/config.c ./src/sound.c ./flagcer/flagser.c ./src/main.c -lm -lasound
./a.out


