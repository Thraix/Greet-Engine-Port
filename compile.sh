#!/bin/bash
g++ src/test.cpp -o main -lGL -l:libglfw.so -l:libfreeimage.so -Ideps/includes
