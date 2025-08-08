#!/usr/bin/env bash

g++ -g -O ./src/main.cpp ./src/inputs.cpp ./src/algorithm.c ./src/mini-gmp.c -o main $(wx-config --cxxflags --libs)

# gcc -g -O ./algorithm.c ./mini-gmp.c -o ./algorithm
# ghc ./Algorithm.hs -o ./Algorithm
