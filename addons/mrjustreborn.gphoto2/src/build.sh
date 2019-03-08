#!bash

gcc -std=c11 -fPIC -c -I ../godot_headers/ simple.c -o simple.o
gcc -shared simple.o -o ../bin/x11/libsimple.so