#!/bin/bash

#compiling and building the project 

SRC=$(ls ./src)
g++ $SRC
chmod 700 ./a.out
./a.out