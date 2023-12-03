#!/bin/sh
#-mwindows
g++ main.cpp -g -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf
x86_64-w64-mingw32-g++ main.cpp -g -static-libgcc -static-libstdc++ -L./lib/ -I./SDL2/include -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf 

