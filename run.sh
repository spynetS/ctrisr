#!/bin/bash

gcc ./src/main.c ./src/Shape.c ./src/render.c ./src/sound.c -L./lib/ -lcanvas -lm -lasound
./a.out
