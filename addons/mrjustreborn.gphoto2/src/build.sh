#!bash

gcc -std=c11 -fPIC -c -I ../godot_headers/ test.c -o test.o
gcc -shared test.o -o ../bin/x11/libtest.so -lgphoto2