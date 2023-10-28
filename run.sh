#!/bin/bash

gcc ./src/main.c ./src/Shape.c -L./lib/ -lcanvas -lm
./a.out


