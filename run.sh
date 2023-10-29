#!/bin/bash

gcc ./src/main.c ./src/Shape.c ./src/render.c -L./lib/ -lcanvas -lm
./a.out
