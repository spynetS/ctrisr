#!/bin/bash

gcc ../printer/src/Canvas.c ../printer/src/msc.c ./src/renderer.c ./src/shapes.c  ./src/config.c ./src/sound.c ./flagcer/flagser.c ./src/main.c -lm -lasound
./a.out


